#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <SDL3/SDL.h>

#include "HelixEngine/Render/OpenGL/Resource.hpp"

//
// void helix::Game::setCommandLineArguments(const int argc, char** argv)
// {
//
// }
//
// int helix::Game::run()
// {
// 	return 0;
// }
//
// helix::Game::Instance::Instance()
// {
//
// }

extern helix::opengl::Shader* helix_test_opengl_shader;

int helix::Game::run()
{
	Window::SDLInit();
	Window::startRun();

	//处理消息循环

	SDL_Event event;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				{
					auto window = static_cast<Window*>(SDL_GetPointerProperty(
							SDL_GetWindowProperties(SDL_GetWindowFromEvent(&event)),
							Window::sdlWindowPointerProperty.data(), nullptr));
					window->destroy();
				}
				break;
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
				default: ;
			}
		}

		helix_test_opengl_shader;
	}
	Window::SDLQuit();
	return 0;
}
