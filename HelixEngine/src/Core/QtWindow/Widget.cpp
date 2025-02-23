#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Node/Scene.hpp>

helix::qt::Widget::Widget(Window* hxWindow) :
	timer(std::make_unique<QTimer>()), hxWindow(hxWindow)
{
	timer->callOnTimeout(this, &Widget::sceneUpdate);
	timer->start(0ms);
	lastFrameTimePoint = SteadyClock::now();
}

void helix::qt::Widget::sceneUpdate()
{
	if (!hxWindow->scene)
		return;

	auto now = SteadyClock::now();
	auto deltaTime = now - lastFrameTimePoint;
	hxWindow->scene->updateScene(deltaTime);
	lastFrameTimePoint = now;
}
