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
			SetPrimitiveTopology,
			SetViewport,
			SetScissor,
			Draw,
			DrawIndexed,

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

			//OpenGL Exclusive
			CreateGLVertexArray,
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};

	struct SharedResourceCommand
	{
		virtual ~SharedResourceCommand() = default;

		enum class Type : int32_t
		{
			Unknown = -1,
			CreateMemoryBuffer,

			//OpenGL Exclusive
			CreateGLShader,
			CreateGLRenderPipeline,
			DestroyGLShader,
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};
}
