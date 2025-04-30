#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Node2D::update(Duration deltaTime)
{
}

void helix::Node2D::render(Renderer* renderer)
{
}

void helix::Node2D::addChild(const Ref<Node2D>& child)
{
	child->intrusiveHook.node = child;
	children.push_back(child->intrusiveHook);
}

void helix::Node2D::removeChild(const Ref<Node2D>& child)
{
	child->intrusiveHook.node = nullptr;
	children.remove_if([&child](const IntrusiveHook& hook) { return child->intrusiveHook.node == hook.node; });
}

helix::Node2D::ChildrenView helix::Node2D::getAllChildren() const
{
	return children | std::views::transform(
			       std::function([](const IntrusiveHook& hook)
			       {
				       return hook.node;
			       }));
}
