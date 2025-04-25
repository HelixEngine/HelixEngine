#include <HelixEngine/Core/QtWindow/Widget.hpp>
#include <HelixEngine/Core/Window.hpp>

helix::qt::Widget::Widget(Window* window) :
	window(window)
{
}

void helix::qt::Widget::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	window->renderer->closeRenderThread();
}
