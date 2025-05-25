#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Render/Resource.hpp>

namespace helix
{
	struct SetRenderPipelineCommand final : RenderCommand
	{
		Ref<RenderPipeline> renderPipeline{};
	};
}
