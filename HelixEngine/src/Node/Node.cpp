#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Node2D::update(Duration deltaTime)
{
}

void helix::Node2D::render(Renderer* renderer)
{
}

void helix::Node2D::addChild(Ref<Node2D>& child)
{
	children.pushBack(child);
}

void helix::Node2D::removeChild(Ref<Node2D>& child)
{
	children.remove(child);
}

helix::IntrusiveList<helix::Ref<helix::Node2D>> helix::Node2D::getAllChildren() const
{
	return children;
}
