#include <Essence/Vulkan/Component/SwapChain.hpp>
#include <Essence/Vulkan/Component/Wsi.hpp>

#ifdef WIN32
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif
#ifdef __APPLE__
//后续把ios和mac分开来
#include <vulkan/vulkan_macos.h>
#endif
#ifdef __linux__
#include <vulkan/vulkan_xcb.h>
#include <vulkan/vulkan_xlib.h>
#endif

helix::Ref<essence::component::SwapChain> essence::vulkan::component::Wsi::makeSwapChain(SwapChain::Property property)
{
	helix::Ref swapChain = new SwapChain;
	auto instance = Device::getVkInstance();
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCI = {};
	surfaceCI.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCI.hwnd = static_cast<HWND>(property.surface);
	surfaceCI.hinstance = GetModuleHandle(nullptr);
	auto createWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(
			instance, "vkCreateWin32SurfaceKHR"));

	if (!createWin32SurfaceKHR)
		helix::Logger::error(Device::vkOutput, u8"Surface无法创建");
	Device::resultProcess(createWin32SurfaceKHR(instance, &surfaceCI, nullptr, &swapChain->surface), u8"Surface无法创建");

#endif
#ifdef __APPLE__
#endif
#ifdef __linux__
#endif

	return swapChain;
}

essence::vulkan::component::SwapChain::~SwapChain()
{
	vkDestroySurfaceKHR(Device::getVkInstance(), surface, nullptr);
}
