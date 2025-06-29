#pragma once
#include <vector>
#include <HelixEngine/Render/Resource.hpp>
#include <glad/glad.h>

namespace helix::opengl
{
	class MemoryBuffer final : public helix::MemoryBuffer
	{
		friend class Renderer;
		GLuint vertexBufferGL{};
	public:
		[[nodiscard]] GLuint getGLBuffer() const;

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

	struct IndexAttribute final
	{
		GLenum elementType = GL_UNSIGNED_INT;
	};

	class VertexArray final : public RenderResource
	{
	public:
		struct Config
		{
			Ref<helix::MemoryBuffer> vertexBuffer;
			Ref<helix::MemoryBuffer> indexBuffer;
			std::vector<VertexAttribute> vertexAttributes;
			IndexAttribute indexAttribute;
		};
	private:
		friend class Renderer;
		GLuint vertexArrayGL{};
		Ref<helix::MemoryBuffer> vertexBuffer;
		Ref<helix::MemoryBuffer> indexBuffer;
		IndexAttribute indexAttribute;
	public:
		[[nodiscard]] GLuint getGLVertexArray() const;
		[[nodiscard]] Ref<helix::MemoryBuffer> getVertexBuffer() const;
	};
}
