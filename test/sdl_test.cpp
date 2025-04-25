#include <HelixEngine/HelixEngine.hpp>

int main()
{
	helix_sdl3::Window window{u8"Hello, HelixEngine", {800, 600}};
	//写一下SDL的main loop
	SDL_Event event;
	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				return 0;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
				return 0;
		}
	}
}
