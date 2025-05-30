#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Render/Command/CreateVertexBufferCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Render/OpenGL/Resource.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <HelixEngine/Render/OpenGL/ExclusiveCommand.hpp>
#include <HelixEngine/Render/Command/SetRenderPipeline.hpp>

namespace helix::opengl
{
	Ref<helix::VertexBuffer> Renderer::createNativeVertexBuffer(VertexBuffer::Usage usage,
	                                                            Ref<MemoryBlock> vertexData) const
	{
		Ref vb = new VertexBuffer;
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
		startRenderThread([=](const std::stop_token& token)
		{
			this->renderToken = token;
			renderLoopFunc();
		});
	}

	void Renderer::renderLoopFunc()
	{
		renderContext = createCurrentSDLContext();
		glInitMtx.lock();
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
		glInitMtx.unlock();

		auto& queue = getRenderQueue();
		auto& pipeline = getResourcePipeline();
		while (!renderToken.stop_requested())
		{
			resourceProc(pipeline->receive());
			renderProc(queue->receive());
		}
		SDL_GL_DestroyContext(renderContext);
		Logger::info(u8"quit render thread");
	}

	SDL_GLContext Renderer::createSDLContext() const
	{
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
		auto pipeline = reinterpret_cast<RenderPipeline*>(cmd->renderPipeline.get());
		glUseProgram(pipeline->getGLProgram());
	}

	void Renderer::resourceProc(const ResourcePipeline::ListRef& list)
	{
		for (auto& cmd: list->getCommands())
		{
			this->resourceCmd = cmd.get();
			switch (cmd->type)
			{
				case ResourceCommand::Type::CreateVertexBuffer:
					createVertexBufferProc();
					break;
				case ResourceCommand::Type::CreateGLShader:
					createGLShaderProc();
					break;
				case ResourceCommand::Type::CreateGLRenderPipeline:
					createGLRenderPipelineProc();
					break;
				case ResourceCommand::Type::DestroyGLShader:
					destroyGLShaderProc();
					break;
				case ResourceCommand::Type::Unknown:
				default:
					Logger::warning(u8"OpenGL Renderer: 未知的ResourceCommand");
					break;
			}
		}
	}

	void Renderer::createVertexBufferProc() const
	{
		auto cvbCmd = resourceCmd->cast<CreateVertexBufferCommand>();
		auto vb = reinterpret_cast<VertexBuffer*>(cvbCmd->vertexBuffer);
		glGenBuffers(1, &vb->vertexBufferGL);
		if (!cvbCmd->vertexData)
			return;
		glBindBuffer(GL_ARRAY_BUFFER, vb->getGLVertexBuffer());
		glBufferData(GL_ARRAY_BUFFER,
		             static_cast<GLsizeiptr>(cvbCmd->vertexData->getSize()),
		             cvbCmd->vertexData->getPointer(),
		             vb->getGLUsage());
	}

	void Renderer::createGLShaderProc() const
	{
		auto cmd = resourceCmd->cast<CreateGLShaderCommand>();
		auto shader = cmd->shader;
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
	}

	void Renderer::createGLRenderPipelineProc() const
	{
		Logger::info(u8"create GLRenderPipeline");
		auto cmd = resourceCmd->cast<CreateGLRenderPipelineCommand>();
		auto pipeline = cmd->renderPipeline;
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
	}

	void Renderer::destroyGLShaderProc() const
	{
		auto cmd = resourceCmd->cast<DestroyGLShaderCommand>();
		glDeleteShader(cmd->shaderGL);
	}

	void Renderer::attachGLShader(const RenderPipeline* pipeline, const helix::Shader* shader)
	{
		glAttachShader(pipeline->getGLProgram(), reinterpret_cast<const Shader*>(shader)->getGLShader());
	}

	Ref<opengl::Shader> Renderer::createGLShader(Shader::Usage usage, std::u8string shaderCode)
	{
		auto shader = createNativeShader(usage);
		CreateGLShaderCommand cmd;
		cmd.type = ResourceCommand::Type::CreateGLShader;
		cmd.shader = shader;
		cmd.shaderCode = std::move(shaderCode);
		getResourcePipeline()->addCommand<CreateGLShaderCommand>(std::move(cmd));
		return shader;
	}

	Ref<opengl::RenderPipeline> Renderer::createGLRenderPipeline(RenderPipeline::Config config) const
	{
		auto renderPipeline = createNativeRenderPipeline();
		CreateGLRenderPipelineCommand cmd;
		cmd.type = ResourceCommand::Type::CreateGLRenderPipeline;
		cmd.config = std::move(config);
		cmd.renderPipeline = renderPipeline;
		getResourcePipeline()->addCommand<CreateGLRenderPipelineCommand>(std::move(cmd));
		return renderPipeline;
	}

	void Renderer::destroyGLShader(const Shader* shader) const
	{
		DestroyGLShaderCommand cmd;
		cmd.type = ResourceCommand::Type::DestroyGLShader;
		cmd.shaderGL = shader->getGLShader();
		getResourcePipeline()->addCommand<DestroyGLShaderCommand>(std::move(cmd));
	}
}
