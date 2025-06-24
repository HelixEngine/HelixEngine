#include <HelixEngine/Node/Scene.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Scene2D::setWindow(Window* window)
{
	this->window = window;
}

void helix::Scene2D::updateScene(Duration deltaTime)
{
	updateAll(deltaTime);
}

void helix::Scene2D::renderScene(Renderer* renderer)
{
	renderAll(renderer);
}

helix::Window* helix::Scene2D::getWindow() const
{
	return window;
}
