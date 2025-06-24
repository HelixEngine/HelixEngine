#pragma once
#include <vector>
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

	struct VertexAttribute final
	{
		GLuint location = 0;
		GLint size = 0;
		GLenum elementType = GL_FLOAT;
		GLboolean normalized = GL_FALSE;
		GLsizei stride = 0;
		void* offset = nullptr;
	};

	class VertexArray final : public Object
	{
	public:
		struct Config
		{
			Ref<helix::VertexBuffer> vertexBuffer;
			std::vector<VertexAttribute> attributes;
		};
	private:
		friend class Renderer;
		GLuint vertexArrayGL{};
		Ref<helix::VertexBuffer> vertexBuffer;
	public:
		[[nodiscard]] GLuint getGLVertexArray() const;
		[[nodiscard]] Ref<helix::VertexBuffer> getVertexBuffer() const;
	};
}
