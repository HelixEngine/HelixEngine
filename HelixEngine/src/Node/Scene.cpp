#include <HelixEngine/Node/Scene.hpp>
#include <HelixEngine/Core/Window.hpp>

helix::Window* helix::SceneBase::getWindow() const
{
	return window;
}

void helix::Scene2D::render(Renderer* renderer)
{
	renderer->begin(getWindow()->getBackgroundColor());
	SceneBase::render(renderer);
}
