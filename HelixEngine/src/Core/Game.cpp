#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <SDL3/SDL.h>

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

int helix::Game::run()
{
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
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
				default: ;
			}
		}
	}
	return 0;
}
