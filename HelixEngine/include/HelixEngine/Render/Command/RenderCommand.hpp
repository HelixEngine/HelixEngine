#pragma once
#include <cstdint>

namespace helix
{
	struct RenderCommand
	{
		virtual ~RenderCommand() = default;

		enum class Type : int32_t
		{
			Unknown = -1,
			Begin,
			End,
			SetRenderPipeline,

			//OpenGL Exclusive
			SetGLVertexArray,
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};

	struct ResourceCommand
	{
		virtual ~ResourceCommand() = default;

		enum class Type : int32_t
		{
			Unknown = -1,
			CreateVertexBuffer,

			//OpenGL Exclusive
			CreateGLShader,
			CreateGLRenderPipeline,
			DestroyGLShader,
			CreateGLVertexArray,
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};
}
