#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Core/Window.hpp>

void helix::Node2D::addNode(Ref<Node2D> child)
{
	child->parent = this;
	child->updateWindowPtr(window);
	children.emplace_back(std::move(child));
}

helix::Window* helix::Node2D::getWindow() const
{
	return window;
}

helix::Node2D* helix::Node2D::getParent() const
{
	return parent;
}

helix::Scene* helix::Node2D::getScene() const
{
	return window->getScene();
}

helix::Ref<helix::Renderer> helix::Node2D::getRenderer() const
{
	return window->getRenderer();
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

void helix::Node2D::updateWindowPtr(Window* window)
{
	this->window = window;
	for (const auto& child: children)
	{
		child->updateWindowPtr(window);
	}
}
