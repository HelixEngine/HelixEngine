#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

class RenderNode final : public Node2D
{
public:
	RenderPipeline* pipeline = nullptr;
	opengl::VertexArray* vertexArray = nullptr;
	Ref<MemoryBuffer> uniformBuffer;

	void render(Renderer* renderer) override
	{
		auto glRenderer = reinterpret_cast<opengl::Renderer*>(renderer);

		auto window = renderer->getWindow();
		Viewport viewport;
		viewport.area = {Vector2F{0.f, 0.f}, Vector2F(window->getSize())};
		Scissor scissor = {Vector2I32{0, 0}, Vector2I32{window->getSize()}};
		glRenderer->setViewport(viewport);
		glRenderer->setScissor(scissor);

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
	};

	VertexData vertexData[] = {
			{{-0.5f, -0.5f}, Color::Blue},
			{{0.5f, -0.5f}, Color::Green},
			{{0.5f, 0.5f}, Color::Red},
			{{-0.5f, 0.5f}, Color::DeepPink},
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

	auto vertexCode =
			u8R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vertexColor;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.f, 1.0);
	vertexColor = aColor;
})";
	auto vertexShader = glRenderer->createGLShader(Shader::Usage::Vertex, vertexCode);

	auto pixelCode =
			u8R"(
#version 420 core
in vec3 vertexColor;
out vec4 FragColor;

layout (std140,binding = 0) uniform UniformBlock
{
	vec4 color;
};

void main()
{
    FragColor = color;
} )";

	auto pixelShader = glRenderer->createGLShader(Shader::Usage::Pixel, pixelCode);

	RenderPipeline::Config config;
	config.vertex = vertexShader;
	config.pixel = pixelShader;

	auto pipeline = glRenderer->createGLRenderPipeline(config);
	// opengl::UniformBindingAttribute uniformBindingAttribute;
	// uniformBindingAttribute.uniformBuffer = uniformBuffer;
	// uniformBindingAttribute.binding = 0;
	// uniformBindingAttribute.offset = 0;
	// uniformBindingAttribute.size = sizeof(Color);
	// glRenderer->setGLUniformBindingAttribute(pipeline, uniformBindingAttribute);
	// glRenderer2->setGLUniformBindingAttribute(pipeline, uniformBindingAttribute);

	vertexShader.reset();
	pixelShader.reset();

	opengl::VertexArray::Config vaConfig;
	vaConfig.vertexBuffer = vertexBuffer;
	vaConfig.indexBuffer = indexBuffer;
	vaConfig.vertexAttributes = {
			{0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr},
			{1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, color))},
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

	auto bitmap = glRenderer->loadBitmap(u8"D:/a.jpg");
	auto texture2d = glRenderer->createTexture2D(bitmap);
	bitmap.reset();
}
