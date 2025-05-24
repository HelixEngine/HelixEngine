#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>

namespace helix
{
	class VertexBuffer;

	struct CreateVertexBufferCommand final : ResourceCommand
	{
		VertexBuffer* vertexBuffer{};
		Ref<MemoryBlock> vertexData{};
	};
}
