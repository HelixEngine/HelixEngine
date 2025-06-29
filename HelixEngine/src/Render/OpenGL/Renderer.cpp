#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Command/GeneralCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Render/OpenGL/Resource.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <HelixEngine/Render/OpenGL/ExclusiveCommand.hpp>

namespace helix::opengl
{
	SDLOpenGLContext::~SDLOpenGLContext()
	{
		SDL_GL_DestroyContext(context);
	}

	Ref<helix::MemoryBuffer> Renderer::createNativeMemoryBuffer(MemoryBuffer::Usage usage,
	                                                            Ref<MemoryBlock> vertexData) const
	{
		Ref vb = new MemoryBuffer;
		vb->setUsage(usage);
		return vb;
	}

	Ref<opengl::Shader> Renderer::createNativeShader(Shader::Usage usage)
	{
		Ref shader = new Shader;
		shader->setUsage(usage);
		shader->renderer = this;
		return shader;
	}

	Ref<opengl::RenderPipeline> Renderer::createNativeRenderPipeline()
	{
		Ref pipeline = new RenderPipeline;
		return pipeline;
	}

	void Renderer::startRun()
	{
		if (!isInitDebugOutput)
			glad_set_post_callback(gladDebugOutput);
		sdlContext->context = createSDLContext();
		makeCurrentContext(nullptr);
	}

	SDL_GLContext Renderer::createSDLContext() const
	{
		for (const auto& window: Window::getAllWindows())
		{
			if (getWindow() == window)
				continue;
			if (window->getGraphicsApi() == GraphicsApi::OpenGL)
			{
				auto glRenderer = reinterpret_cast<Renderer*>(window->getRenderer().get());
				if (!glRenderer->sdlContext)
					continue;
				SDL_GL_MakeCurrent(window->getSDLWindow(), glRenderer->sdlContext->context);
				break;
			}
		}
		if (!SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1))
			Window::sdlError(u8"OpenGL Renderer: 设置OpenGL共享上下文失败");
		auto ctx = SDL_GL_CreateContext(getWindow()->getSDLWindow());
		if (ctx == nullptr)
			Window::sdlError(u8"OpenGL Renderer: 创建OpenGL上下文失败");
		return ctx;
	}

	void Renderer::makeCurrentContext(SDL_GLContext context) const
	{
		if (!SDL_GL_MakeCurrent(getWindow()->getSDLWindow(), context))
			Window::sdlError(u8"OpenGL Renderer: 设置OpenGL上下文失败");
	}

	SDL_GLContext Renderer::createCurrentSDLContext() const
	{
		auto context = createSDLContext();
		makeCurrentContext(context);
		return context;
	}

	void Renderer::renderProc(const RenderQueue::ListRef& list)
	{
		for (auto& cmd: list->getCommands())
		{
			this->renderCmd = cmd.get();
			switch (cmd->type)
			{
				case RenderCommand::Type::Begin:
					beginProc();
					break;
				case RenderCommand::Type::End:
					endProc();
					break;
				case RenderCommand::Type::SetRenderPipeline:
					setRenderPipelineProc();
					break;
				case RenderCommand::Type::SetGLVertexArray:
					setGLVertexArrayProc();
					break;
				case RenderCommand::Type::SetPrimitiveTopology:
					setPrimitiveTopologyProc();
					break;
				case RenderCommand::Type::SetViewport:
					setViewportProc();
					break;
				case RenderCommand::Type::SetScissor:
					setScissorProc();
					break;
				case RenderCommand::Type::Draw:
					drawProc();
					break;
				case RenderCommand::Type::DrawIndexed:
					drawIndexedProc();
					break;
				case RenderCommand::Type::CreateGLVertexArray:
					createGLVertexArrayProc();
					break;
				case RenderCommand::Type::CreateMemoryBuffer:
					createMemoryBufferProc();
					break;
				case RenderCommand::Type::CreateGLShader:
					createGLShaderProc();
					break;
				case RenderCommand::Type::CreateGLRenderPipeline:
					createGLRenderPipelineProc();
					break;
				case RenderCommand::Type::DestroyGLShader:
					destroyGLShaderProc();
					break;
				case RenderCommand::Type::Unknown:
				default:
					Logger::warning(u8"OpenGL Renderer: 未知的RenderCommand");
					break;
			}
		}
	}

	void Renderer::beginProc() const
	{
		auto beginCmd = renderCmd->cast<BeginCommand>();
		glClearColor(beginCmd->clearColor.r,
		             beginCmd->clearColor.g,
		             beginCmd->clearColor.b,
		             beginCmd->clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::endProc() const
	{
		SDL_GL_SwapWindow(getWindow()->getSDLWindow());
	}

	void Renderer::setRenderPipelineProc() const
	{
		auto cmd = renderCmd->cast<SetRenderPipelineCommand>();
		if (auto pipeline = reinterpret_cast<RenderPipeline*>(cmd->renderPipeline.get()))
		{
			pipeline->usable();
			glUseProgram(pipeline->getGLProgram());
			return;
		}
		glUseProgram(0);
	}

	void Renderer::setPrimitiveTopologyProc()
	{
		auto cmd = renderCmd->cast<SetPrimitiveTopologyCommand>();
		primitiveTopology = cmd->primitiveTopology;
	}

	void Renderer::setViewportProc() const
	{
		auto cmd = renderCmd->cast<SetViewportCommand>();
		if (cmd->viewports.empty())
		{
			Logger::warning(u8"OpenGL Renderer: Viewport数组为空");
			return;
		}
		if (cmd->viewports.size() == 1)
		{
			auto viewport = cmd->viewports[0];
			glViewport(static_cast<GLint>(viewport.area.position.x), static_cast<GLint>(viewport.area.position.y),
			           static_cast<GLsizei>(viewport.area.size.x), static_cast<GLsizei>(viewport.area.size.y));
			return;
		}

		std::vector<GLfloat> viewportsGL(cmd->viewports.size() * 4);
		for (size_t i = 0; i < cmd->viewports.size(); ++i)
		{
			const auto& viewport = cmd->viewports[i];
			viewportsGL[i * 4] = viewport.area.position.x;
			viewportsGL[i * 4 + 1] = viewport.area.position.y;
			viewportsGL[i * 4 + 2] = viewport.area.size.x;
			viewportsGL[i * 4 + 3] = viewport.area.size.y;
		}

		glViewportArrayv(cmd->firstIndex, static_cast<GLsizei>(cmd->viewports.size()),
		                 reinterpret_cast<const GLfloat*>(cmd->viewports.data()));
	}

	void Renderer::setScissorProc() const
	{
		auto cmd = renderCmd->cast<SetScissorCommand>();
		if (cmd->scissors.empty())
		{
			Logger::warning(u8"OpenGL Renderer: Scissor数组为空");
			return;
		}
		if (cmd->scissors.size() == 1)
		{
			auto scissor = cmd->scissors[0];
			glScissor(scissor.position.x,
			          scissor.position.y,
			          scissor.size.x,
			          scissor.size.y);
			return;
		}

		glScissorArrayv(cmd->firstIndex, static_cast<GLsizei>(cmd->scissors.size()),
		                reinterpret_cast<const GLint*>(cmd->scissors.data()));
	}

	void Renderer::drawProc() const
	{
		auto cmd = renderCmd->cast<DrawCommand>();
		glDrawArrays(getGLPrimitiveTopology(primitiveTopology), 0, static_cast<GLsizei>(cmd->vertexCount));
	}

	void Renderer::drawIndexedProc() const
	{
		auto cmd = renderCmd->cast<DrawIndexedCommand>();
		glDrawElements(getGLPrimitiveTopology(primitiveTopology), static_cast<GLsizei>(cmd->indexCount),
		               indexAttribute.elementType, nullptr);
	}

	void Renderer::setGLVertexArrayProc()
	{
		auto cmd = renderCmd->cast<SetGLVertexArrayCommand>();
		if (auto vertexArray = cmd->vertexArray.get())
		{
			glBindVertexArray(vertexArray->getGLVertexArray());
			if (vertexArray->indexBuffer)
			{
				indexAttribute = vertexArray->indexAttribute;
			}
			return;
		}
		glBindVertexArray(0);
	}

	void Renderer::createGLVertexArrayProc() const
	{
		auto cmd = renderCmd->cast<CreateGLVertexArrayCommand>();
		auto vertexArray = cmd->vertexArray;

		if (vertexArray->getGLVertexArray())
			return;

		glGenVertexArrays(1, &vertexArray->vertexArrayGL);
		glBindVertexArray(vertexArray->getGLVertexArray());

		//Vertex Buffer
		if (auto vb = reinterpret_cast<MemoryBuffer*>(cmd->config.vertexBuffer.get()))
		{
			vb->usable();
			glBindBuffer(GL_ARRAY_BUFFER, vb->getGLBuffer());
			vertexArray->vertexBuffer = cmd->config.vertexBuffer;
		} else
			Logger::error(u8"在创建OpenGL Vertex Array时，传入的Vertex Buffer不可为nullptr");

		//Index Buffer
		if (auto ib = reinterpret_cast<MemoryBuffer*>(cmd->config.indexBuffer.get()))
		{
			ib->usable();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->getGLBuffer());
			vertexArray->indexBuffer = cmd->config.indexBuffer;
			vertexArray->indexAttribute = cmd->config.indexAttribute;
		}

		//Vertex Attribute
		for (const auto& attr: cmd->config.vertexAttributes)
		{
			glEnableVertexAttribArray(attr.location);
			glVertexAttribPointer(attr.location, attr.size, attr.elementType,
			                      attr.normalized, attr.stride, attr.offset);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		vertexArray->notify();
	}


	void Renderer::createMemoryBufferProc() const
	{
		auto cmd = renderCmd->cast<CreateMemoryBufferCommand>();
		auto buf = reinterpret_cast<MemoryBuffer*>(cmd->memoryBuffer.get());

		if (buf->getGLBuffer())
			return;

		glGenBuffers(1, &buf->vertexBufferGL);
		if (!cmd->bufferData)
			return;
		glBindBuffer(GL_ARRAY_BUFFER, buf->getGLBuffer());
		glBufferData(GL_ARRAY_BUFFER,
		             static_cast<GLsizeiptr>(cmd->bufferData->getSize()),
		             cmd->bufferData->getPointer(),
		             buf->getGLUsage());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		buf->notify();
	}

	void Renderer::createGLShaderProc() const
	{
		auto cmd = renderCmd->cast<CreateGLShaderCommand>();
		auto shader = cmd->shader;

		if (shader->getGLShader())
			return;

		shader->shaderGL = glCreateShader(shader->getGLUsage());

		const auto length = static_cast<GLint>(cmd->shaderCode.size());
		auto code = cmd->shaderCode.data();
		glShaderSource(shader->shaderGL, 1, reinterpret_cast<const GLchar* const*>(&code), &length);
		glCompileShader(shader->shaderGL);

		int success;
		glGetShaderiv(shader->shaderGL, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int infoSize{};
			glGetShaderiv(shader->shaderGL, GL_INFO_LOG_LENGTH, &infoSize);
			std::u8string infoLog(infoSize, '\0');
			glGetShaderInfoLog(shader->shaderGL, infoSize, nullptr, reinterpret_cast<GLchar*>(infoLog.data()));
			std::u8string_view shaderType;
			switch (shader->getUsage())
			{
				case Shader::Usage::Vertex:
					shaderType = u8"顶点";
					break;
				case Shader::Usage::Pixel:
					shaderType = u8"像素";
					break;
				default:
					Logger::warning(u8"OpenGL Shader: 无法识别的着色器类型");
			}
			Logger::error(u8"OpenGL Shader: 编译", shaderType, u8"着色器失败，错误信息: \n", infoLog);
		}
		shader->notify();
	}

	void Renderer::createGLRenderPipelineProc() const
	{
		auto cmd = renderCmd->cast<CreateGLRenderPipelineCommand>();
		auto pipeline = cmd->renderPipeline;

		if (pipeline->getGLProgram())
			return;

		pipeline->programGL = glCreateProgram();
		attachGLShader(pipeline, cmd->config.vertex);
		attachGLShader(pipeline, cmd->config.pixel);
		glLinkProgram(pipeline->programGL);

		int success;
		glGetProgramiv(pipeline->programGL, GL_LINK_STATUS, &success);
		if (!success)
		{
			int infoSize{};
			glGetProgramiv(pipeline->programGL, GL_INFO_LOG_LENGTH, &infoSize);
			std::u8string infoLog(infoSize, '\0');
			glGetProgramInfoLog(pipeline->programGL, infoSize, nullptr, reinterpret_cast<GLchar*>(infoLog.data()));
			Logger::error(u8"OpenGL Shader: 链接GL Program失败，错误信息: \n", infoLog);
		}
		pipeline->notify();
	}

	void Renderer::destroyGLShaderProc() const
	{
		auto cmd = renderCmd->cast<DestroyGLShaderCommand>();
		if (cmd->shaderGL == 0)
			return;
		glDeleteShader(cmd->shaderGL);
		cmd->shaderGL = 0;
	}

	void Renderer::attachGLShader(const RenderPipeline* pipeline, const helix::Shader* shader)
	{
		glAttachShader(pipeline->getGLProgram(), reinterpret_cast<const Shader*>(shader)->getGLShader());
	}

	GLenum Renderer::getGLPrimitiveTopology(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::TriangleList:
				return GL_TRIANGLES;
			default:
				break;
		}
		Logger::warning(u8"无法识别的PrimitiveTopology，默认使用TriangleList");
		return GL_TRIANGLES;
	}

	Ref<opengl::Shader> Renderer::createGLShader(Shader::Usage usage, std::u8string shaderCode)
	{
		auto shader = createNativeShader(usage);
		CreateGLShaderCommand cmd;
		cmd.type = RenderCommand::Type::CreateGLShader;
		cmd.shader = shader;
		cmd.shaderCode = std::move(shaderCode);
		getRenderQueue()->addCommand<CreateGLShaderCommand>(std::move(cmd));
		return shader;
	}

	Ref<opengl::RenderPipeline> Renderer::createGLRenderPipeline(RenderPipeline::Config config) const
	{
		auto renderPipeline = createNativeRenderPipeline();
		CreateGLRenderPipelineCommand cmd;
		cmd.type = RenderCommand::Type::CreateGLRenderPipeline;
		cmd.config = std::move(config);
		cmd.renderPipeline = renderPipeline;
		getRenderQueue()->addCommand<CreateGLRenderPipelineCommand>(std::move(cmd));
		return renderPipeline;
	}

	Ref<opengl::VertexArray> Renderer::createGLVertexArray(VertexArray::Config config) const
	{
		Ref vertexArray = new VertexArray;
		CreateGLVertexArrayCommand cmd;
		cmd.type = RenderCommand::Type::CreateGLVertexArray;
		cmd.config = std::move(config);
		cmd.vertexArray = vertexArray;
		getRenderQueue()->addCommand<CreateGLVertexArrayCommand>(std::move(cmd));
		return vertexArray;
	}

	void Renderer::setGLVertexArray(Ref<VertexArray> vertexArray) const
	{
		SetGLVertexArrayCommand cmd;
		cmd.type = RenderCommand::Type::SetGLVertexArray;
		cmd.vertexArray = std::move(vertexArray);
		getRenderQueue()->addCommand<SetGLVertexArrayCommand>(std::move(cmd));
	}

	void Renderer::destroyGLShader(const Shader* shader) const
	{
		DestroyGLShaderCommand cmd;
		cmd.type = RenderCommand::Type::DestroyGLShader;
		cmd.shaderGL = shader->getGLShader();
		getRenderQueue()->addCommand<DestroyGLShaderCommand>(std::move(cmd));
	}

	void Renderer::readyRender()
	{
		makeCurrentContext(sdlContext->context);
		glInitMtx.lock();
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
		glInitMtx.unlock();
	}

	std::mutex sharedResourceMtx;

	void Renderer::renderThreadFunc(const std::stop_token& token)
	{
		makeCurrentContext(sdlContext->context);
		SDL_GL_SetSwapInterval(0);
		Duration time;
		size_t fps = 0;
		auto lastTime = SteadyClock::now();
		while (!token.stop_requested())
		{
			renderProc(getRenderQueue()->receive());
			auto now = SteadyClock::now();
			time += now - lastTime;
			lastTime = now;
			++fps;
			if (time >= 1s)
			{
				Logger::info(u8"FPS: ", fps);
				time -= 1s;
				fps = 0;
			}
		}
	}

	void Renderer::gladDebugOutput(const char* name, void* funcPtr, int lenArgs, ...)
	{
		GLenum errorCode = glad_glGetError();
		std::u8string_view errorType;
		switch (errorCode)
		{
			case GL_NO_ERROR:
				return;
			case GL_INVALID_ENUM:
				errorType = u8"GL_INVALID_ENUM";
				break;
			case GL_INVALID_INDEX:
				errorType = u8"GL_INVALID_INDEX";
				break;
			case GL_INVALID_OPERATION:
				errorType = u8"GL_INVALID_OPERATION";
				break;
			case GL_INVALID_VALUE:
				errorType = u8"GL_INVALID_VALUE";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorType = u8"GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			default:
				errorType = u8"未知错误";
				break;
		}
		Logger::error(u8"在调用OpenGL函数（", std::u8string_view(reinterpret_cast<const char8_t*>(name)), u8"）时，发生了错误：",
		              errorType);
	}

}
