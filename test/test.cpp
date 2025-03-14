#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <thread>
#include <Essence/Essence.hpp>

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

	essence::Device::setComponentLoader({
			essence::component::Wsi::getLoader(),
			essence::component::DebugUtil::getLoader(),
	});

	Ref<essence::component::DebugUtil> debugUtil;
	essence::Device::findGlobalComponent(debugUtil);
	debugUtil->setMessageOutput();
	Ref<essence::component::Wsi> wsi;
	essence::Device::findDeviceComponent(wsi);

	auto devices = essence::Device::getDevice();
	for (const auto& device: devices)
	{
		Logger::info(device->getName());
	}
	Window::Property property;
	property.title = u8"你好，HelixEngine";
	property.size = {800, 600};
	// property.flag.setItem(Window::Flag::MaximumButton, false);
	// property.flag.setItem(Window::Flag::MinimumButton, false);
	Ref window = new Window(property);
	Ref scene = new Scene;
	scene->addNode(new FpsNode);
	window->enter(scene);

	window->setFixedSize(true);
	window->setSize({500, 600});

	return Game::run();
}
