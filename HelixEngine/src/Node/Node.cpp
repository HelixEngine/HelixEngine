#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Util/Logger.hpp>

void helix::Node2D::update(Duration deltaTime)
{
}

void helix::Node2D::render(Renderer* renderer)
{
}

void helix::Node2D::updateAll(Duration deltaTime)
{
	//以后可能要加入Layer的顺序控制
	update(deltaTime);
	for (auto& child: children)
	{
		child->updateAll(deltaTime);
	}
}

void helix::Node2D::renderAll(Renderer* renderer)
{
	//以后可能要加入Layer的顺序控制
	render(renderer);
	for (auto& child: children)
	{
		child->renderAll(renderer);
	}
}

void helix::Node2D::addChild(const Ref<Node2D>& child)
{
	if (child->parent)
	{
		Logger::warning(u8"Node2D::addChild: "
		                "子节点(", child->getName(), u8")"
		                "已有父节点(", child->getParent()->getName(), u8")");
		return;
	}
	children.pushBack(child);
	child->parent = this;
}

void helix::Node2D::removeChildImmediate(Ref<Node2D>& child)
{
	child->parent = nullptr;
	children.remove(child);
}

helix::Ref<helix::Node2D> helix::Node2D::findChild(std::u8string_view name)
{
	auto it = std::ranges::find_if(children, [name](const auto& child)
	{
		return child->getName() == name;
	});
	if (it != children.end())
	{
		return *it;
	}

	return nullptr;
}

const helix::IntrusiveList<helix::Ref<helix::Node2D>>& helix::Node2D::getAllChildren() const
{
	return children;
}

helix::Node2D* helix::Node2D::getParent() const
{
	return parent;
}

const helix::Transform2D& helix::Node2D::getTransform() const
{
	return transform;
}
