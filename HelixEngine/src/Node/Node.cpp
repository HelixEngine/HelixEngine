#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Node2D::addNode(const Ref<Node2D>& child)
{
	child->intrusiveHook.node = child;
	children.push_back(child->intrusiveHook);
	// auto a = children | std::views::transform([](const IntrusiveHook& hook) { return hook.node; });
	// for (const auto& node2d: a)
	// {
	//
	// }
}

void helix::Node2D::removeNode(const Ref<Node2D>& child)
{
	children.remove(child->intrusiveHook);
}
