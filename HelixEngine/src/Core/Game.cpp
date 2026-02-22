#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <SDL3/SDL.h>

#ifdef WIN32
#include <Windows.h>
#endif

void helix::Game::init()
{
#ifdef WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
	Window::SDLInit();
}

int helix::Game::run()
{
	state = State::Start;
	Window::start();

	if (startCallback.has_value())
	{
		startCallback.value()();
		startCallback.reset();
	}

	state = State::Run;

	if (Window::getAllWindows().empty())
		state = State::Quit;

	SDL_Event event;

	while (state == State::Run)
	{
		//处理消息循环
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				// case SDL_EVENT_WINDOW_RESIZED:
				// case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				// {
				// 	auto window = static_cast<Window*>(SDL_GetPointerProperty(
				// 			SDL_GetWindowProperties(SDL_GetWindowFromEvent(&event)),
				// 			Window::sdlWindowPointerProperty.data(), nullptr));
				// 	int w,h;
				// 	SDL_GetWindowSize(window->getSDLWindow(),&w,&h);
				// 	window->property.size.x = w;
				// 	window->property.size.y = h;
				// 	Logger::info(u8"width:",w,u8"height:",h);
				// 	break;
				// }
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				{
					auto window = static_cast<Window*>(SDL_GetPointerProperty(
							SDL_GetWindowProperties(SDL_GetWindowFromEvent(&event)),
							Window::sdlWindowPointerProperty.data(), nullptr));
					window->destroy();
				}
				break;
				case SDL_EVENT_QUIT:
					state = State::Quit;
					break;
				default: ;
			}
		}
	}
	Window::SDLQuit();
	return 0;
}

void helix::Game::setStartCallback(StartCallback callback)
{
	startCallback = std::move(callback);
}

helix::Game::State helix::Game::getState()
{
	return state;
}

extern void setup();

int main()
{
	helix::Game::init();
	setup();
	return helix::Game::run();
}
