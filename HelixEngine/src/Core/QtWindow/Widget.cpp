#include <mutex>
#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>
#include <HelixEngine/Util/Time.hpp>

helix::qt::Widget::Widget(Window* hxWindow) :
	updateTimer(std::make_unique<QTimer>()), renderTimer(std::make_unique<QTimer>()), hxWindow(hxWindow)
{
	// renderTimer->callOnTimeout(this, &Widget::sceneRender);
	// renderTimer->start(0ms);
	// updateTimer->callOnTimeout(this, &Widget::sceneUpdate);
	// updateTimer->start(0ms);
	lastFrameTimePoint = SteadyClock::now();
}

void helix::qt::Widget::sceneUpdate()
{
	auto scene = property(Window::qtScenePropertyName).value<Ref<Scene>>();
	if (!scene)
		return;

	auto now = SteadyClock::now();
	auto deltaTime = now - lastFrameTimePoint;
	scene->updateScene(deltaTime);
	lastFrameTimePoint = now;

	//update();
}

void helix::qt::Widget::sceneRender()
{
	auto scene = property(Window::qtScenePropertyName).value<Ref<Scene>>();
	if (!scene)
		return;
	scene->renderScene();

	//repaint();
}
