#include <HelixEngine/Node/Node.hpp>

void helix::Node::addNode(Ref<Node> child)
{
	children.emplace_back(std::move(child));
}
