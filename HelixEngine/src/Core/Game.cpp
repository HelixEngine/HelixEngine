#include <HelixEngine/Core/Game.hpp>

void helix::Game::setCommandLineArguments(const int argc, char** argv)
{
	Instance::getInstance().argc = argc;
	Instance::getInstance().argv = argv;
}

int helix::Game::run()
{
	return QApplication::exec();
}

helix::Game::Instance::Instance()
{
	qApplication = std::make_unique<QApplication>(argc, argv);
}
