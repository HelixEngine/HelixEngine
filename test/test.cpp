#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <QtWidgets/qapplication.h>

using namespace helix;

int main(int argc, char** argv)
{
	Vector2F vec;
	vec.x = 1;
	vec.ktmVec = 1;
	const auto* app = new QApplication{argc, argv};
	Ref window = new Window(u8"你好，HelixEngine", 800, 600);

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

	return QApplication::exec();
}
