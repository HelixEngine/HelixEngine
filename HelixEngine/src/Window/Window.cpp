#include <SDL3/SDL.h>
#include <HelixEngine/Core/Window.hpp>

helix::Window::Window(const std::u8string_view title, const int32_t width, const int32_t height)
{
	qtWidget = new QWidget;
	qtWidget->setWindowTitle(title.data());
	qtWidget->resize(width, height);
	qtWidget->show();
}
