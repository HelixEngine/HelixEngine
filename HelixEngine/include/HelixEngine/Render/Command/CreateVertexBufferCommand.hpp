#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>

namespace helix
{
	class VertexBuffer;

	struct CreateVertexBufferCommand final : ResourceCommand
	{
		CreateVertexBufferCommand() { type = Type::CreateVertexBuffer; }
		VertexBuffer* vertexBuffer{};
		Ref<MemoryBlock> vertexData{};
	};
}
