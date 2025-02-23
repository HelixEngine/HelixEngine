#pragma once
#include <HelixEngine/Base/Singleton.hpp>
#include <nvrhi/nvrhi.h>

namespace helix::render
{
	class Context final
	{
		enum class Platform
		{
			D3d12,
			Vulkan,
		};

		struct Instance final : Singleton<Instance>
		{
			Instance();
			nvrhi::DeviceHandle device;
		};

		static inline Platform platform = Platform::Vulkan;
	};
}
