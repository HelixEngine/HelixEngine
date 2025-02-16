#include <HelixEngine/Core/Game.hpp>

void helix::Game::init(int argc, char** argv)
{
	qApplication = std::make_unique<QApplication>(argc, argv);
}

int helix::Game::run()
{
	return QApplication::exec();
}
