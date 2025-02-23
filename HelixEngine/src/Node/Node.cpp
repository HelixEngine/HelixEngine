#include <HelixEngine/Node/Node.hpp>

void helix::Node::addNode(Ref<Node> child)
{
	children.emplace_back(std::move(child));
}

void helix::Node::update(const Duration deltaTime)
{
	onTick(deltaTime);
	for (const auto& child: children)
	{
		child->update(deltaTime);
	}
}
