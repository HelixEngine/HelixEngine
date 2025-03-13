#include <vector>
#include <Essence/Vulkan/Component/Wsi.hpp>

void essence::vulkan::component::Wsi::Loader::load(
		Device::InstanceProperty& instanceProperty)
{
	bool isSupportedVulkanSurface = false;
	std::string wsiPlatformExtension;
	for (const auto& extension: instanceProperty.extensions) //感谢GLFW的源码帮助~
	{
		if (std::string extensionName = extension.extensionName;
			extensionName == "VK_KHR_surface")
			isSupportedVulkanSurface = true;
		else if (
#ifdef WIN32
			extensionName == "VK_KHR_win32_surface" ||
#endif
#ifdef __APPLE__
			extensionName == "VK_MVK_macos_surface" ||
			extensionName == "VK_EXT_metal_surface" ||
#endif
#ifdef __linux__
			extensionName == "VK_KHR_xlib_surface" ||
			extensionName == "VK_KHR_xcb_surface" ||
			extensionName == "VK_KHR_wayland_surface" ||
#endif
			extensionName == "VK_EXT_headless_surface")
			wsiPlatformExtension = std::move(extensionName);
	}

	if (!isSupportedVulkanSurface || wsiPlatformExtension.empty())
		return;

	wsi = new Wsi;

	instanceProperty.enabledExtensions.insert(
			instanceProperty.enabledExtensions.end(),
			{
					"VK_KHR_surface",
					wsiPlatformExtension.c_str(),
			});
	wsi->platformExtensionName = std::move(wsiPlatformExtension);
	helix::Logger::info(u8"Wsi 全局部分加载成功");
}

helix::Ref<essence::Component> essence::vulkan::component::Wsi::Loader::load(VkPhysicalDevice physicalDevice)
{
	if (!wsi)
		return nullptr;
	return wsi;
}
