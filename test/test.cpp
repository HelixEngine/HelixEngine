#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <ktm/ktm.h>

using namespace helix;

int main()
{
	Window window(u8"HelixEngine", 800, 600);

	bool isDone = false;
	SDL_Event event;
	while (!isDone)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				isDone = true;
				break;
			default:
				break;
		}
	}
}
