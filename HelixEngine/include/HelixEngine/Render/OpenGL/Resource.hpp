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
}
