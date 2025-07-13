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
			CreateMemoryBuffer,
			LoadBitmap,
			CreateTexture2DFromBitmap,
			CreateSampler,

			//OpenGL Exclusive
			SetGLVertexArray,
			SetGLUniformBindingAttribute,
			CreateGLVertexArray,
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
