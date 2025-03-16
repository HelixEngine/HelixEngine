#pragma once
#include <Essence/Component/SwapChain.hpp>
#include <vulkan/vulkan.h>

namespace essence::vulkan::component
{
	class SwapChain final : public essence::component::SwapChain
	{
		friend class Wsi;
		VkSurfaceKHR surface = nullptr;
		VkSwapchainKHR swapChain = nullptr;
	public:
		~SwapChain();
	};
}
