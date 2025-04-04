#pragma once
#include <cstdint>

namespace helix
{
	enum class MessageLevel
	{
		Info,
		Warning,
		Error,
	};

	enum class RenderFeature : uint32_t
	{
		DebugUtil = 0b1,
		SwapChain = 0b10,
		Default = DebugUtil | SwapChain,
	};
}
