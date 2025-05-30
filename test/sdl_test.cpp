#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

RenderPipeline* helix_test_render_pipeline = nullptr;

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref window2 = new Window{u8"Hello, HelixEngine2", {800, 600}};
	window2->setName(u8"opengl2");
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

	auto glRenderer = reinterpret_cast<opengl::Renderer*>(window->getRenderer().get());
	auto vertexCode =
			u8R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";
	auto vertexShader = glRenderer->createGLShader(Shader::Usage::Vertex, vertexCode);

	auto pixelCode =
			u8R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} )";

	auto pixelShader = glRenderer->createGLShader(Shader::Usage::Pixel, pixelCode);

	RenderPipeline::Config config;
	config.vertex = vertexShader;
	config.pixel = pixelShader;

	auto pipeline = glRenderer->createGLRenderPipeline(std::move(config));

	vertexShader.reset();
	pixelShader.reset();

	helix_test_render_pipeline = pipeline;

	//写一下SDL的main loop
	return Game::run();
}

