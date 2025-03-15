#include <vector>
#include <Essence/Vulkan/Component/Wsi.hpp>

void essence::vulkan::component::Wsi::Loader::load(
		InstanceProperty& instanceProperty)
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
	helix::Logger::info(u8"Wsi Global部分加载成功");
}

void essence::vulkan::component::Wsi::Loader::load(DeviceProperty& deviceProperty)
{
	const auto it = std::ranges::find_if(deviceProperty.extensions,
	                                     [](const VkExtensionProperties& properties)
	                                     {
		                                     return std::string(properties.extensionName) ==
		                                            VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	                                     });
	if (it != deviceProperty.extensions.end())
	{
		helix::Logger::info(u8"Wsi Device部分加载成功");;
		wsi->device = deviceProperty.device;
		deviceProperty.enabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceProperty.deviceComponents.push_back(wsi);
	}
}
