#include <HelixEngine/Node/Scene.hpp>

void helix::Scene::updateScene(Duration deltaTime)
{
	update(deltaTime);
}

void helix::Scene::renderScene()
{
	auto renderer = getRenderer();
	renderer->begin(Color::Red);
	render();
	renderer->end();
}
