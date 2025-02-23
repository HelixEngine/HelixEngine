#include <HelixEngine/Node/Scene.hpp>

void helix::Scene::updateScene(Duration deltaTime)
{
	update(std::move(deltaTime));
}
