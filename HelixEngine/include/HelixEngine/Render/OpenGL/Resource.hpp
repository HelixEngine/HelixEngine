#pragma once
#include <vector>
#include <HelixEngine/Render/Resource.hpp>
#include <glad/glad.h>

namespace helix::opengl
{
	class Renderer;

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
		opengl::Renderer* renderer = nullptr;
	public:
		[[nodiscard]] GLuint getGLShader() const;

		[[nodiscard]] GLenum getGLUsage() const;

		~Shader() override;
	};

	struct UniformBindingAttribute final
	{
		Ref<helix::MemoryBuffer> uniformBuffer;
		GLuint binding = 0;
		GLuint offset = 0;
		GLuint size = 0;
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
		[[nodiscard]] Ref<helix::MemoryBuffer> getIndexBuffer() const;
		[[nodiscard]] const IndexAttribute& getIndexAttribute() const;
	};

	class Texture2D final : public helix::Texture2D
	{
		friend class Renderer;
		GLuint textureGL{};
	public:
		[[nodiscard]] GLuint getGLTexture() const;
	private:
		static GLint getGLColorFormat(const PixelFormat& format);
		static GLint getGLStorageType(const PixelFormat& format);
		static GLint getGLPixelFormat(const PixelFormat& format);
	};

	class Sampler final : public helix::Sampler
	{
		friend class Renderer;
		GLuint samplerGL{};
	public:
		[[nodiscard]] GLuint getGLSampler() const;
	private:
		static GLint getGLFilter(Filter filter);
		static GLint getGLFilterWithMipmap(GLint minFilter, Filter filter);
		static GLint getGLWarp(Warp warp);
	};

	struct Texture2DUnit final
	{
		GLuint unitIndex = 0;
		Ref<helix::Texture2D> texture2d{};
		Ref<helix::Sampler> sampler{};
	};
}
