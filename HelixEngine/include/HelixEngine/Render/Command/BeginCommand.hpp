#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <HelixEngine/Util/Color.hpp>

namespace helix
{
	struct BeginCommand final : RenderCommand
	{
		BeginCommand() { type = Type::Begin; }
		Color clearColor;
		//后续添加RenderTarget成员以指定渲染目标，nullptr表示swapChain当前target，离屏渲染则需手动指定
	};
}
