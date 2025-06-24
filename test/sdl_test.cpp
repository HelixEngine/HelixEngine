#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;


class RenderNode final : public Node2D
{
public:
	RenderPipeline* pipeline = nullptr;
	opengl::VertexArray* vertexArray = nullptr;

	void render(Renderer* renderer) override
	{
		auto glRenderer = reinterpret_cast<opengl::Renderer*>(renderer);
		glRenderer->setRenderPipeline(pipeline);
		glRenderer->setGLVertexArray(vertexArray);
		Logger::info(u8"render!");
	}
};

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref window2 = new Window{u8"Hello, HelixEngine2", {800, 600}};
	window2->setName(u8"opengl2");
	Ref scene = new Scene2D;
	window->enter(scene);

	Ref renderNode = new RenderNode;
	scene->addChild(renderNode);

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

	opengl::VertexArray::Config vaConfig;
	vaConfig.vertexBuffer = vertexBuffer;
	vaConfig.attributes = {
			{0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr}
	};

	auto vao = glRenderer->createGLVertexArray(vaConfig);

	renderNode->pipeline = pipeline;
	renderNode->vertexArray = vao;

	//写一下SDL的main loop
	return Game::run();
}

