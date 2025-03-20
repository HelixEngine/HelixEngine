#include <mutex>
#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Node/Scene.hpp>

helix::qt::Widget::Widget(Window* hxWindow) :
	updateTimer(std::make_unique<QTimer>()), renderTimer(std::make_unique<QTimer>()), hxWindow(hxWindow)
{
	renderTimer->callOnTimeout(this, &Widget::sceneRender);
	renderTimer->start(0ms);
	updateTimer->callOnTimeout(this, &Widget::sceneUpdate);
	updateTimer->start(0ms);
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

	update();
}

void helix::qt::Widget::sceneRender()
{
	if (!hxWindow->scene)
		return;
	hxWindow->scene->renderScene();

	repaint();
}
