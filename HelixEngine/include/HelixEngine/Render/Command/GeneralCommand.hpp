#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Color.hpp>

namespace helix
{
	class MemoryBlock;
	class RenderPipeline;
	class VertexBuffer;

	struct BeginCommand final : RenderCommand
	{
		Color clearColor;
		//后续添加RenderTarget成员以指定渲染目标，nullptr表示swapChain当前target，离屏渲染则需手动指定
	};

	struct CreateVertexBufferCommand final : SharedResourceCommand
	{
		VertexBuffer* vertexBuffer{};
		Ref<MemoryBlock> vertexData{};
	};

	struct SetRenderPipelineCommand final : RenderCommand
	{
		Ref<RenderPipeline> renderPipeline{};
	};

	struct SetPrimitiveTopologyCommand final : RenderCommand
	{
		PrimitiveTopology primitiveTopology{};
	};

	struct SetViewportCommand final : RenderCommand
	{
		uint32_t firstIndex = 0;
		std::vector<Viewport> viewports;
	};

	struct SetScissorCommand final : RenderCommand
	{
		uint32_t firstIndex = 0;
		std::vector<Scissor> scissors;
	};

	struct DrawCommand final : RenderCommand
	{
		uint32_t vertexCount{};
	};
}
