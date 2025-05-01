#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "HelixEngine/Util/Logger.hpp"

namespace helix::opengl
{
	void Renderer::startRun()
	{
		startRenderThread([=](const std::stop_token& token)
		{
			LoopData loopData{token, getWindow(), getRenderQueue()};
			renderLoopFunc(loopData);
		});
	}

	void Renderer::renderLoopFunc(const LoopData& loopData)
	{
		auto& queue = loopData.renderQueue;
		auto* window = loopData.window;
		auto* sdlWindow = window->getSDLWindow();

		SDL_GLContext context = SDL_GL_CreateContext(sdlWindow);
		SDL_GL_MakeCurrent(sdlWindow, context);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

		while (!loopData.token.stop_requested())
		{
			RenderQueue::ListRef list = queue->receive();

			for (auto& cmd: list->getCommands())
			{
				switch (cmd->type)
				{
					case RenderCommand::Type::Begin:
					{
						auto beginCmd = cmd->cast<BeginCommand>();
						glClearColor(beginCmd->clearColor.r,
						             beginCmd->clearColor.g,
						             beginCmd->clearColor.b,
						             beginCmd->clearColor.a);
						glClear(GL_COLOR_BUFFER_BIT);
						break;
					}
					case RenderCommand::Type::End:
						SDL_GL_SwapWindow(sdlWindow);
						break;
					default:
						break;
				}
			}
		}
	}
}
