#include <HelixEngine/Node/Collection.hpp>

void helix::Collection2D::addNode(const Ref<Node2D>& node)
{
	hooks.push_back(node->intrusiveHook);
}

std::vector<helix::Node2D*> helix::Collection2D::getNodes() const
{
	std::vector<Node2D*> result{hooks.size()};
	size_t index = 0;
	for (const auto& hook: hooks)
	{
		index++;
		result[index] = hook.node;
	}
	return result;
}

void helix::Collection2D::update(Duration deltaTime)
{
	for (const auto& hook: hooks)
	{
		hook.node->update(deltaTime);
	}
}

void helix::Collection2D::render(Renderer* renderer)
{
	for (const auto& hook: hooks)
	{
		hook.node->render(renderer);
	}
}
