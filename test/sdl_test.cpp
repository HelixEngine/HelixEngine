#include <iostream>
#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

void post_call_callback_default(const char* name, void* funcPtr, int lenArgs, ...)
{
	GLenum errorCode = glad_glGetError();
	std::u8string_view errorType;
	switch (errorCode)
	{
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			errorType = u8"GL_INVALID_ENUM";
			break;
		case GL_INVALID_INDEX:
			errorType = u8"GL_INVALID_INDEX";
			break;
		case GL_INVALID_OPERATION:
			errorType = u8"GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorType = u8"GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errorType = u8"未知错误";
			break;
	}
	Logger::error(errorType, u8"，调用了OpenGL函数：", std::u8string_view(reinterpret_cast<const char8_t*>(name)));
}

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
		glRenderer->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		glRenderer->draw(3);
	}
};

int main()
{
	glad_set_post_callback(post_call_callback_default);
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref window2 = new Window{u8"Hello, HelixEngine2", {800, 600}};
	window2->setName(u8"opengl2");
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
			{{0.f, 0.5f}, Color::Red},
	};

	auto vertexBuffer = window->getRenderer()->createVertexBuffer(
			VertexBuffer::Usage::Static,
			new MemoryBlock{vertexData, sizeof(vertexData)});

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
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor,1.f);
} )";

	auto pixelShader = glRenderer->createGLShader(Shader::Usage::Pixel, pixelCode);

	RenderPipeline::Config config;
	config.vertex = vertexShader;
	config.pixel = pixelShader;

	auto pipeline = opengl::Renderer::createGLRenderPipeline(config);

	vertexShader.reset();
	pixelShader.reset();

	opengl::VertexArray::Config vaConfig;
	vaConfig.vertexBuffer = vertexBuffer;
	vaConfig.attributes = {
			{0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr},
			{1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, color))},
	};

	auto vao = glRenderer->createGLVertexArray(vaConfig);
	auto vao2 = glRenderer2->createGLVertexArray(vaConfig);

	renderNode->pipeline = pipeline;
	renderNode->vertexArray = vao;

	renderNode2->pipeline = pipeline;
	renderNode2->vertexArray = vao2;

	//写一下SDL的main loop
	return Game::run();
}
