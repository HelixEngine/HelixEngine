#include <SDL3/SDL.h>
#include <HelixEngine/Core/Window.hpp>

helix::Window::Window(const std::u8string_view title, const int32_t initialWidth, const int32_t initialHeight) :
	Window(title, Vector2I32{initialWidth, initialHeight})
{
}

helix::Window::Window(const std::u8string_view title, const Vector2I32 initialSize) :
	Window(CreateInfo{.title = title.data(), .initialSize = initialSize})
{
}

helix::Window::Window(const CreateInfo& info)
{
	qWidget = new QWidget(info.parent ? info.parent->qWidget : nullptr);
	qWidget->resize(static_cast<QSize>(info.initialSize));
	qWidget->setWindowTitle(info.title.c_str());
	if (info.isShow)
		qWidget->show();
}

void helix::Window::resize(const Vector2I32 newSize) const
{
	qWidget->resize(QSize{newSize});
}

void helix::Window::setFixedSize(const Vector2I32 fixedSize) const
{
	qWidget->setFixedSize(QSize{fixedSize});
}

void helix::Window::setFixedSize(const bool isFixed) const
{
	if (isFixed)
		qWidget->setFixedSize(qWidget->size());
	else
	{
		qWidget->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
		qWidget->setMinimumSize(0, 0);
	}
}

helix::Vector2I32 helix::Window::getSize() const
{
	return Vector2I32{qWidget->size().width(), qWidget->size().height()};
}

const QWidget* helix::Window::getQWidget() const
{
	return qWidget;
}
