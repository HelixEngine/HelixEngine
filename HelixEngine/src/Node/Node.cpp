#include <HelixEngine/Node/Node.hpp>

void helix::Node2D::addNode(Ref<Node2D> child)
{
	children.emplace_back(std::move(child));
}

void helix::Node2D::update(const Duration deltaTime)
{
	onTick(deltaTime);
	for (const auto& child: children)
	{
		child->update(deltaTime);
	}
}

void helix::Node2D::render()
{
	onRender();
	for (const auto& child: children)
	{
		child->render();
	}
}
