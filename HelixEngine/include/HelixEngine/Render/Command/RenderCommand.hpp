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
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};
}
