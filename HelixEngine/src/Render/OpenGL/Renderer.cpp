#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <HelixEngine/Util/Logger.hpp>

namespace helix::opengl
{
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
		auto& queue = getRenderQueue();

		initRender();

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
				default:
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
}
