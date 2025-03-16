#pragma once
#include <HelixEngine/Util/Object.hpp>

namespace essence::component
{
	using Surface = void*;

	class SwapChain : public helix::Object
	{
	public:
		struct Property
		{
			Surface surface;
			uint32_t framebufferCount;
			//Format...
		};
	};
}
