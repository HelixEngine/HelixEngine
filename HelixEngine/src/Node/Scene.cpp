#include <HelixEngine/Node/Scene.hpp>

void helix::Scene::updateScene(Duration deltaTime)
{
	update(deltaTime);
}

void helix::Scene::renderScene()
{
	render();
}
