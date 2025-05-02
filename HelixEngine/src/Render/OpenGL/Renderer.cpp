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
	Ref<helix::VertexBuffer> Renderer::createVertexBuffer(VertexBuffer::Usage usage,
	                                                      Ref<MemoryBlock> vertexData) const
	{
		Ref vb = new VertexBuffer;
		vb->setUsage(usage);

		CreateVertexBufferCommand cmd;
		cmd.type = RenderCommand::Type::CreateVertexBuffer;
		cmd.vertexBuffer = vb;
		cmd.vertexData = std::move(vertexData);
		getRenderQueue()->addCommand<CreateVertexBufferCommand>(cmd);
		return vb;
	}

	void Renderer::startRun()
	{
		startRenderThread([=](const std::stop_token& token)
		{
			this->token = token;
			renderLoopFunc();
		});
	}

	void Renderer::renderLoopFunc()
	{
		initRender();

		auto& queue = getRenderQueue();
		while (!token.stop_requested())
			cmdProc(queue->receive());
	}

	void Renderer::initRender()
	{
		auto* sdlWindow = getWindow()->getSDLWindow();

		context = SDL_GL_CreateContext(sdlWindow);
		SDL_GL_MakeCurrent(sdlWindow, context);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
	}

	void Renderer::cmdProc(const RenderQueue::ListRef& list)
	{
		for (auto& cmd: list->getCommands())
		{
			this->cmd = cmd.get();
			switch (cmd->type)
			{
				case RenderCommand::Type::Begin:
					beginProc();
					break;
				case RenderCommand::Type::End:
					endProc();
					break;
				case RenderCommand::Type::CreateVertexBuffer:
					createVertexBufferProc();
					break;
				default:
					Logger::warning(u8"OpenGL Renderer: 未知的RenderCommand");
					break;
			}
		}
	}

	void Renderer::beginProc() const
	{
		auto beginCmd = cmd->cast<BeginCommand>();
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

	void Renderer::createVertexBufferProc() const
	{
		auto cvbCmd = cmd->cast<CreateVertexBufferCommand>();
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
