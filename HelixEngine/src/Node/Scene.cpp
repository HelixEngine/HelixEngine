#include <HelixEngine/Node/Scene.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Scene2D::setWindow(Window* window)
{
	this->window = window;
}

helix::Window* helix::Scene2D::getWindow() const
{
	return window;
}
