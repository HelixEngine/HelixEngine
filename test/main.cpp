#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

#include "HelixEngine/Render/Shader.hpp"

using namespace helix;

class RenderNode final : public Node2D
{
public:
	RenderPipeline* pipeline = nullptr;
	opengl::VertexArray* vertexArray = nullptr;
	Ref<MemoryBuffer> uniformBuffer;
	Ref<Texture2D> texture2d;
	Ref<Sampler> sampler;

	void render(Renderer* renderer) override
	{
		auto glRenderer = reinterpret_cast<opengl::Renderer*>(renderer);

		auto window = renderer->getWindow();
		Viewport viewport;
		viewport.area = {Vector2F{0.f, 0.f}, Vector2F(window->getSize())};
		Scissor scissor = {Vector2I32{0, 0}, Vector2I32{window->getSize()}};
		glRenderer->setViewport(viewport);
		glRenderer->setScissor(scissor);

		glRenderer->setGLTexture2DUnit({0, texture2d, sampler});
		glRenderer->setRenderPipeline(pipeline);

		opengl::UniformBindingAttribute uniformBindingAttribute;
		uniformBindingAttribute.uniformBuffer = uniformBuffer;
		uniformBindingAttribute.binding = 0;
		uniformBindingAttribute.offset = 0;
		uniformBindingAttribute.size = sizeof(Color);
		glRenderer->setGLUniformBindingAttribute(uniformBindingAttribute);

		glRenderer->setGLVertexArray(vertexArray);
		glRenderer->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		glRenderer->drawIndexed(6);
	}
};

struct PixelInput
{
	helix::shader::Float4 vertexColor;
	helix::shader::Float2 TexCoord;
};

void setup()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	window->setName(u8"window1");
	Ref window2 = new Window{u8"Hello, HelixEngine2", {800, 600}};
	window2->setName(u8"window2");
	Ref scene = new Scene2D;
	window->enter(scene);
	Ref scene2 = new Scene2D;
	window2->enter(scene2);

	Ref renderNode = new RenderNode;
	scene->addChild(renderNode);

	Ref renderNode2 = new RenderNode;
	scene2->addChild(renderNode2);

	struct VertexData
	{
		Vector2F position;
		Color color;
		Vector2F texCoord;
	};

	VertexData vertexData[] = {
			{{-0.5f, -0.5f}, Color::Blue, {0.f, 0.f}},
			{{0.5f, -0.5f}, Color::Green, {1.0f, 0.0f}},
			{{0.5f, 0.5f}, Color::Red, {1.f, 1.f}},
			{{-0.5f, 0.5f}, Color::DeepPink, {0.f, 1.f}},
	};

	uint32_t indexData[] = {
			0, 1, 2,
			0, 3, 2
	};

	auto vertexBuffer = window->getRenderer()->createMemoryBuffer(
			MemoryBuffer::Type::Geometry,
			MemoryBuffer::Usage::Static, MemoryBlock::clone(vertexData, sizeof(vertexData)));
	auto indexBuffer = window->getRenderer()->createMemoryBuffer(
			MemoryBuffer::Type::Geometry,
			MemoryBuffer::Usage::Static, MemoryBlock::clone(indexData, sizeof(indexData)));

	Color uniformColor = Color::Blue;
	auto uniformBuffer = window->getRenderer()->createMemoryBuffer(
			MemoryBuffer::Type::Uniform,
			MemoryBuffer::Usage::Dynamic, MemoryBlock::clone(&uniformColor, sizeof(uniformColor)));
	uniformColor = Color::Red;
	auto uniformBuffer2 = window->getRenderer()->createMemoryBuffer(
			MemoryBuffer::Type::Uniform,
			MemoryBuffer::Usage::Dynamic, MemoryBlock::clone(&uniformColor, sizeof(uniformColor)));

	auto glRenderer = reinterpret_cast<opengl::Renderer*>(window->getRenderer().get());
	auto glRenderer2 = reinterpret_cast<opengl::Renderer*>(window2->getRenderer().get());

	auto vertex = [&](helix::shader::Float2 aPos,
	                  const helix::shader::Float4& aColor,
	                  const helix::shader::Float2& aTexCoord)
	{
		using namespace helix::shader;
		position() = Float4(aPos->x, aPos->y, 0.f, 1.f);
		Aggregate<PixelInput> pixelInput;
		pixelInput->vertexColor = aColor;
		pixelInput->TexCoord = aTexCoord;
		return pixelInput;
	};

	helix::shader::Texture2D<ktm::fvec4> ourTexture;
	helix::shader::Sampler ourTextureSampler;
	helix::shader::Float4 color;
	auto pixel = [&](helix::shader::Aggregate<PixelInput> input)
	{
		using namespace helix::shader;
		return mix(mix(ourTexture.sample(ourTextureSampler, input->TexCoord), input->vertexColor, 0.5f),
		           color, 0.2f);
	};

	auto vertexCode =
			u8R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec4 vertexColor;
out vec2 TexCoord;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.f, 1.0);
	vertexColor = aColor;
	TexCoord = aTexCoord;
})";
	auto vertexShader = glRenderer->createGLShader(Shader::Usage::Vertex, vertexCode);

	auto pixelCode =
			u8R"(
#version 420 core
in vec4 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

layout (std140,binding = 0) uniform UniformBlock
{
	vec4 color;
};

uniform sampler2D ourTexture;

void main()
{
    FragColor = mix(mix(texture(ourTexture, TexCoord), vertexColor,0.5),color,0.2);
} )";

	auto pixelShader = glRenderer->createGLShader(Shader::Usage::Pixel, pixelCode);

	RenderPipeline::Config config;
	config.vertex = vertexShader;
	config.pixel = pixelShader;

	auto pipeline = glRenderer->createGLRenderPipeline(config);

	vertexShader.reset();
	pixelShader.reset();

	opengl::VertexArray::Config vaConfig;
	vaConfig.vertexBuffer = vertexBuffer;
	vaConfig.indexBuffer = indexBuffer;
	vaConfig.vertexAttributes = {
			{0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr},
			{1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, color))},
			{2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, texCoord))},
	};
	vaConfig.indexAttribute = {GL_UNSIGNED_INT};

	auto vao = glRenderer->createGLVertexArray(vaConfig);
	auto vao2 = glRenderer2->createGLVertexArray(vaConfig);

	renderNode->pipeline = pipeline;
	renderNode->vertexArray = vao;
	renderNode->uniformBuffer = uniformBuffer;

	renderNode2->pipeline = pipeline;
	renderNode2->vertexArray = vao2;
	renderNode2->uniformBuffer = uniformBuffer2;

	auto image = glRenderer->loadImage(u8"D:/b.jpg");
	auto texture2d = image->getTexture2D();
	auto sampler = glRenderer->createSampler();

	renderNode->texture2d = texture2d;
	renderNode2->texture2d = texture2d;

	renderNode->sampler = sampler;
	renderNode2->sampler = sampler;

	Game::setStartCallback([=]
	{
		texture2d->usable();
		window->setSize(texture2d->getSize());
		window2->setSize(texture2d->getSize());
	});
}
