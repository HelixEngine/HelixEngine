#include <HelixEngine/Node/Scene.hpp>
#include <HelixEngine/Core/Window.hpp>

helix::Window* helix::SceneBase::getWindow() const
{
	return window;
}
