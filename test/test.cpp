#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <QtWidgets/qapplication.h>
#include <QScreen>

using namespace helix;

int main(int argc, char** argv)
{
	Game::init(argc, argv);
	Window::Property property;
	property.title = u8"你好，HelixEngine";
	property.size = {800, 600};
	// property.flag.setItem(Window::Flag::MaximumButton, false);
	// property.flag.setItem(Window::Flag::MinimumButton, false);
	Ref window = new Window(property);

	window->setFixedSize(true);
	window->setSize({500, 600});

	return Game::run();
}
