#pragma once
#include <cstdint>

namespace helix
{
	struct RenderCommand
	{
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
		enum class Type : int32_t
		{
			Unknown = -1,
			CreateVertexBuffer,

			//OpenGL Exclusive
			CreateGLShader,
			CreateGLRenderPipeline,
		};

		Type type = Type::Unknown;

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};
}
