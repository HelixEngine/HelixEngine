#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Render/Command/CreateVertexBufferCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Render/OpenGL/Resource.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace helix::opengl
{
	Ref<helix::VertexBuffer> Renderer::createNativeVertexBuffer(VertexBuffer::Usage usage,
	                                                            Ref<MemoryBlock> vertexData) const
	{
		Ref vb = new VertexBuffer;
		vb->setUsage(usage);
		return vb;
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
			renderProc(queue->receive());
			resourceProc(pipeline->receive());
		}
		SDL_GL_DestroyContext(renderContext);
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
}
