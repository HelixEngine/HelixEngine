#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <SDL3/SDL.h>

int helix::Game::run()
{
	Window::SDLInit();

	Window::startRun();

	SDL_Event event;

	while (isRunning)
	{
		//处理消息循环
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
	}
	Window::SDLQuit();
	return 0;
}

bool helix::Game::isQuit()
{
	return !isRunning;
}

extern void setup();

int main()
{
	setup();
	return helix::Game::run();
}
