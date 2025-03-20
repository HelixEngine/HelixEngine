#include <HelixEngine/Node/RectNode.hpp>
#include <HelixEngine/Util/Logger.hpp>

void helix::RectNode::onRender()
{
	Node2D::onRender();
	Logger::info(u8"RectNode Rendering!");
}
