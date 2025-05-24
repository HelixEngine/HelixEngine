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
		[[nodiscard]] GLuint getGLVertexBuffer() const;

		[[nodiscard]] GLenum getGLUsage() const;
	};

	class Shader final : public helix::Shader
	{
		friend class Renderer;
		GLuint shaderGL{};
		Renderer* renderer = nullptr;
	public:
		[[nodiscard]] GLuint getGLShader() const;

		[[nodiscard]] GLenum getGLUsage() const;

		~Shader() override;
	};

	class RenderPipeline final : public helix::RenderPipeline
	{
		friend class Renderer;
		GLuint programGL{};
	public:
		[[nodiscard]] GLuint getGLProgram() const;
	};
}
