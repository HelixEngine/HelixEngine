#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref scene = new Scene2D;
	window->enter(scene);

	std::vector vertexData = {
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
	};

	auto vertexBuffer = window->getRenderer()->createVertexBuffer(
			VertexBuffer::Usage::Static,
			new MemoryBlock{vertexData.data(), vertexData.size() * sizeof(float)});

	//写一下SDL的main loop
	return Game::run();
}
