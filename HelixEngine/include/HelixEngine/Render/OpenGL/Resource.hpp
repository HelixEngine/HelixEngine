#pragma once
#include <HelixEngine/Render/Resource.hpp>
#include <glad/glad.h>

namespace helix::opengl
{
	class VertexBuffer final : public helix::VertexBuffer
	{
		friend class Renderer;
		GLuint vertexBufferGL{};
	public:
		[[nodiscard]] GLuint getGLVertexBuffer() const
		{
			return vertexBufferGL;
		}

		[[nodiscard]] GLenum getGLUsage() const
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
	};

	class Shader final : public helix::Shader
	{
		friend class Renderer;
		GLuint shaderGL{};
	public:
		[[nodiscard]] GLuint getGLShader() const
		{
			return shaderGL;
		}

		[[nodiscard]] GLenum getGLUsage() const
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

		~Shader() override
		{
			Logger::info(u8"destroy");
			if (shaderGL)
			{
				glDeleteShader(shaderGL);
			}
		}
	};

	class RenderPipeline final : public helix::RenderPipeline
	{
		friend class Renderer;
		GLuint programGL{};
	public:
		[[nodiscard]] GLuint getGLProgram() const
		{
			return programGL;
		}
	};
}
