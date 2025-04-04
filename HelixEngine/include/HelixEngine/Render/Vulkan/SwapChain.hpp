#pragma once
#include <HelixEngine/Render/Vulkan/RenderInstance.hpp>

namespace helix::vulkan
{
	//用于配置交换链和Surface支持的功能类
	class SwapChain final
	{
		static inline std::string platformExtensionName;
		SwapChain() = default;
	public:
		explicit SwapChain(WId windowId);
		//配置Surface支持的功能
		static void config(InstanceProperty& instanceProperty);
		static void config(DeviceProperty& deviceProperty);

		VkSwapchainKHR swapChain{};
		VkSurfaceKHR surface{};
		static VkSurfaceKHR createSurface(WId windowId);
		static VkSwapchainKHR createSwapChain();
	};
}
