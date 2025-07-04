#include <HelixEngine/Render/OpenGL/Resource.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

GLuint helix::opengl::MemoryBuffer::getGLBuffer() const
{
	return vertexBufferGL;
}

GLenum helix::opengl::MemoryBuffer::getGLUsage() const
{
	switch (getUsage())
	{
		case Usage::Static:
			switch (getType())
			{

				case Type::Geometry:
					return GL_STATIC_DRAW;
				case Type::Uniform:
					return GL_STATIC_READ;
				case Type::Storage:
					return GL_STATIC_COPY;
				default:
					break;
			}
			return GL_STATIC_DRAW;
		case Usage::Dynamic:
			switch (getType())
			{

				case Type::Geometry:
					return GL_DYNAMIC_DRAW;
				case Type::Uniform:
					return GL_DYNAMIC_READ;
				case Type::Storage:
					return GL_DYNAMIC_COPY;
				default:
					break;
			}
		case Usage::Stream:
			switch (getType())
			{
				case Type::Geometry:
					return GL_STREAM_DRAW;
				case Type::Uniform:
					return GL_STREAM_READ;
				case Type::Storage:
					return GL_STREAM_COPY;
				default:
					break;
			}
		default:
			break;
	}
	Logger::error(u8"无法获取的OpenGL Usage");
	return 0;
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
	usable();
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

helix::Ref<helix::MemoryBuffer> helix::opengl::VertexArray::getVertexBuffer() const
{
	return vertexBuffer;
}
