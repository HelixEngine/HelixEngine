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

helix::Ref<helix::MemoryBuffer> helix::opengl::VertexArray::getIndexBuffer() const
{
	return indexBuffer;
}

const helix::opengl::IndexAttribute& helix::opengl::VertexArray::getIndexAttribute() const
{
	return indexAttribute;
}

GLuint helix::opengl::Texture2D::getGLTexture() const
{
	return textureGL;
}

GLint helix::opengl::Texture2D::getGLColorFormat(const PixelFormat& format)
{
	switch (format.colorFormat)
	{
		case PixelFormat::ColorFormat::RGBA:
			return GL_RGBA;
		case PixelFormat::ColorFormat::RGB:
			return GL_RGB;
		case PixelFormat::ColorFormat::RG:
			return GL_RG;
		case PixelFormat::ColorFormat::R:
			return GL_R;
		case PixelFormat::ColorFormat::BGRA:
			return GL_BGRA;
		case PixelFormat::ColorFormat::BGRX: [[fallthrough]];
		case PixelFormat::ColorFormat::Unknown: [[fallthrough]];
		default:
			break;
	}
	return -1; //Unknown
}

GLint helix::opengl::Texture2D::getGLStorageType(const PixelFormat& format)
{
	switch (format.storageType[0])
	{
		case PixelFormat::StorageType::Float:
			return GL_FLOAT;
		case PixelFormat::StorageType::UInt:
			return GL_UNSIGNED_INT;
		case PixelFormat::StorageType::SInt:
			return GL_INT;
		case PixelFormat::StorageType::UNorm:
			if (format.bitWidth[0] == 8)
				return GL_UNSIGNED_BYTE;
			if (format.bitWidth[0] == 16)
				return GL_UNSIGNED_SHORT;
		//return GL_UNSIGNED_NORMALIZED;
		case PixelFormat::StorageType::SNorm:
			if (format.bitWidth[0] == 8)
				return GL_BYTE;
			if (format.bitWidth[0] == 16)
				return GL_SHORT;
		//return GL_SIGNED_NORMALIZED;
		case PixelFormat::StorageType::Typeless:
			Logger::error(u8"无法从Typeless格式获取OpenGL存储类型！");
			break;
		default:
			break;
	}
	return -1;
}

GLint helix::opengl::Texture2D::getGLPixelFormat(const PixelFormat& format)
{
	if (format == PixelFormat::Unknown)
		return -1; //Unknown
	if (format == PixelFormat::RGBA8UNorm)
		return GL_RGBA8;
	if (format == PixelFormat::RGBA8UNormSRGB)
		return GL_SRGB8_ALPHA8;
	if (format == PixelFormat::RGBA16UNorm)
		return GL_RGBA16;
	if (format == PixelFormat::RGBA16Float)
		return GL_RGBA16F;
	if (format == PixelFormat::RGBA32Float)
		return GL_RGBA32F;
	if (format == PixelFormat::RGB32Float)
		return GL_RGB32F;
	if (format == PixelFormat::RG32Float)
		return GL_RG32F;
	if (format == PixelFormat::R32Float)
		return GL_R32F;
	if (format == PixelFormat::BGRA8UNorm)
		return GL_BGRA;
	// if (format == PixelFormat::BGRX8UNorm)
	// 	return -1; //Unknown
	return -1; //Unknown
}

GLuint helix::opengl::Sampler::getGLSampler() const
{
	return samplerGL;
}

GLint helix::opengl::Sampler::getGLFilter(Filter filter)
{
	switch (filter)
	{
		case Filter::Linear:
			return GL_LINEAR;
			break;
		case Filter::Nearest:
			return GL_NEAREST;
			break;
	}
	return -1;
}

GLint helix::opengl::Sampler::getGLFilterWithMipmap(GLint minFilter, Filter filter)
{
	switch (filter)
	{
		case Filter::Linear:
			switch (minFilter)
			{
				case GL_LINEAR:
					return GL_LINEAR_MIPMAP_LINEAR;
				case GL_NEAREST:
					return GL_LINEAR_MIPMAP_NEAREST;
				default:
					break;
			}
			break;
		case Filter::Nearest:
			switch (minFilter)
			{
				case GL_LINEAR:
					return GL_NEAREST_MIPMAP_LINEAR;
				case GL_NEAREST:
					return GL_NEAREST_MIPMAP_NEAREST;
				default:
					break;
			}
			break;
	}
	return -1;
}

GLint helix::opengl::Sampler::getGLWarp(Warp warp)
{
	switch (warp)
	{
		case Warp::Repeat:
			return GL_REPEAT;
		case Warp::Mirror:
			return GL_MIRRORED_REPEAT;
		case Warp::ClampEdge:
			return GL_CLAMP_TO_EDGE;
		case Warp::ClampBorder:
			return GL_CLAMP_TO_BORDER;
	}
	return -1;
}
