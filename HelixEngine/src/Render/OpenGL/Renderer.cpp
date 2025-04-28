#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace helix
{
	struct BeginCommand;
}

namespace helix::opengl
{
	Renderer::Renderer(helix::Window* window)
	{
		LoopData loopData{};
		loopData.window = window;
		loopData.renderQueue = getRenderQueue();
		renderThread = std::jthread{renderLoopFunc, loopData};
	}

	void Renderer::renderLoopFunc(const LoopData& loopData)
	{
		auto& queue = loopData.renderQueue;
		auto* window = loopData.window;
		auto* sdlWindow = window->getSDLWindow();

		//SDL_GL_MakeCurrent(sdlWindow, reinterpret_cast<RenderContext*>(window->context.get())->sdlGlContext);
		SDL_GLContext context = SDL_GL_CreateContext(sdlWindow);
		SDL_GL_MakeCurrent(sdlWindow, context);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

		RenderQueue::ListRef list;
		while (true)
		{
			if (std::optional opt = queue->receive(); opt.has_value())
				list = std::move(opt.value());
			else
				break;

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
