#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <SDL3/SDL_events.h>
#include <QtWidgets/qapplication.h>

using namespace helix;

int main(int argc, char** argv)
{

	auto* app = new QApplication{argc, argv};
	Window window(u8"HelixEngine", 800, 600);


	// bool isDone = false;
	// SDL_Event event;
	//
	// while (!isDone)
	// {
	// 	SDL_WaitEvent(&event);
	// 	switch (event.type)
	// 	{
	// 		case SDL_EVENT_QUIT:
	// 			isDone = true;
	// 			break;
	// 		default:
	// 			break;
	// 	}
	// }
	app->exec();
}
