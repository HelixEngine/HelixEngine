#include <HelixEngine/Render/OpenGL/Resource.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

GLuint helix::opengl::VertexBuffer::getGLVertexBuffer() const
{
	return vertexBufferGL;
}

GLenum helix::opengl::VertexBuffer::getGLUsage() const
{
	switch (getUsage())
	{
		case Usage::Static:
			return GL_STATIC_DRAW;
		case Usage::Dynamic:
			return GL_DYNAMIC_DRAW;
		case Usage::Stream:
			return GL_STREAM_DRAW;
		default:
			Logger::error(u8"无法识别的OpenGL Usage");
			return 0;
	}
}

GLuint helix::opengl::Shader::getGLShader() const
{
	return shaderGL;
}

GLenum helix::opengl::Shader::getGLUsage() const
{
	switch (getUsage())
	{
		case Usage::Vertex:
			return GL_VERTEX_SHADER;
		case Usage::Pixel:
			return GL_FRAGMENT_SHADER;
		default:
			Logger::error(u8"无法识别的OpenGL Shader Usage");
			return 0;
	}
}

helix::opengl::Shader::~Shader()
{
	renderer->destroyGLShader(this);
}

GLuint helix::opengl::RenderPipeline::getGLProgram() const
{
	return programGL;
}

GLuint helix::opengl::VertexArray::getGLVertexArray() const
{
	return vertexArrayGL;
}

helix::Ref<helix::VertexBuffer> helix::opengl::VertexArray::getVertexBuffer() const
{
	return vertexBuffer;
}
