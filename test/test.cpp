#include <HelixEngine/HelixEngine.hpp>
#include <iostream>
#include <thread>
#include <HelixEngine/Render/QRhi/RenderInstance.hpp>

using namespace helix;

class FpsNode final : public Node2D
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

#include <HelixEngine/Render/Vulkan/RenderInstance.hpp>

int main(int argc, char** argv)
{
	Game::setCommandLineArguments(argc, argv);

	Renderer::setRenderInstance(new q_rhi::RenderInstance);

	//Window----------------------------------
	Window::Property property;
	property.title = u8"你好，HelixEngine";
	property.size = {800, 600};
	//property.flag.setItem(Window::Flag::MaximumButton, false);
	//property.flag.setItem(Window::Flag::MinimumButton, false);
	Ref window = new Window(property);
	Ref scene = new Scene;
	scene->addNode(new FpsNode);
	scene->addNode(new RectNode);
	window->enter(scene);

	window->setFixedSize(true);
	window->setSize({500, 600});
	//Window----------------------------------

	return Game::run();
}
