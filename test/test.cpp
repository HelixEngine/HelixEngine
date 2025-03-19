#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <thread>

using namespace helix;

class FpsNode final : public Node
{
public:
	void onTick(Duration deltaTime) override
	{
		time += deltaTime;
		fps++;
		if (time >= 1s)
		{
			time -= 1s;
			Logger::info(u8"Fps: ", fps);
			fps = 0;
		}
	}

	Duration time;
	uint64_t fps{};
};

int main(int argc, char** argv)
{
	Game::setCommandLineArguments(argc, argv);

	//Window----------------------------------
	Window::Property property;
	property.title = u8"你好，HelixEngine";
	property.size = {800, 600};
	//property.flag.setItem(Window::Flag::MaximumButton, false);
	//property.flag.setItem(Window::Flag::MinimumButton, false);
	Ref window = new Window(property);
	Ref scene = new Scene;
	scene->addNode(new FpsNode);
	window->enter(scene);

	window->setFixedSize(true);
	window->setSize({500, 600});
	//Window----------------------------------

	return Game::run();
}
