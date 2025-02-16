#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <QtWidgets/qapplication.h>
#include <QScreen>

using namespace helix;

int main(int argc, char** argv)
{
	const auto* app = new QApplication{argc, argv};
	Window::Property property;
	property.title = u8"你好，HelixEngine";
	property.size = {800, 600};
	// property.flag.setItem(Window::Flag::MaximumButton, false);
	// property.flag.setItem(Window::Flag::MinimumButton, false);
	Ref window = new Window(property);
	property.title = u8"child";
	property.size = {300, 300};
	//property.parent = window.get();
	Ref window2 = new Window(property);

	window->setFixedSize(true);
	window->setSize({500, 600});

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
