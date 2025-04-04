#include <HelixEngine/Render/Vulkan/SwapChain.hpp>
#include <QVulkanInstance>
#include <QWindow>
#include <QVulkanWindow>

helix::vulkan::SwapChain::SwapChain(WId windowId)
{
	surface = createSurface(windowId);
	swapChain = createSwapChain();
}

void helix::vulkan::SwapChain::config(InstanceProperty& instanceProperty)
{
	bool isSupportedVulkanSurface = false;
	std::string platformExtension;
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
			extensionName == "VK_KHR_xlib_surface" ||	// 这里要对xlib、xcb、wayland分别处理
			extensionName == "VK_KHR_xcb_surface" ||
			extensionName == "VK_KHR_wayland_surface" ||
#endif
			extensionName == "VK_EXT_headless_surface")
			platformExtension = std::move(extensionName);
	}

	if (!isSupportedVulkanSurface || platformExtension.empty())
		Logger::error(RenderInstance::vkOutput, u8"不支持Surface");

	instanceProperty.enabledExtensions.insert(
			instanceProperty.enabledExtensions.end(),
			{
					"VK_KHR_surface",
					platformExtension.c_str(),
			});
	platformExtensionName = std::move(platformExtension);
}

void helix::vulkan::SwapChain::config(DeviceProperty& deviceProperty)
{
	if (platformExtensionName.empty())
		return;
	const auto it = std::ranges::find_if(deviceProperty.extensions,
	                                     [](const VkExtensionProperties& properties)
	                                     {
		                                     return std::string(properties.extensionName) ==
		                                            VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	                                     });
	if (it == deviceProperty.extensions.end())
		return;

	deviceProperty.enabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	deviceProperty.renderFeatures.setItem(RenderFeature::SwapChain, true);
}

VkSurfaceKHR helix::vulkan::SwapChain::createSurface(WId windowId)
{
	VkSurfaceKHR surface{};
	auto instance = RenderInstance::getVkInstance();
	QVulkanInstance qVkInstance;
	qVkInstance.setVkInstance(instance);
	if (!qVkInstance.create())
	{
		Logger::error(RenderInstance::vkOutput, u8"无法创建QVulkanInstance");
		return surface;
	}
	auto qWindow = QWindow::fromWinId(windowId);
	qWindow->setVulkanInstance(&qVkInstance);
	surface = QVulkanInstance::surfaceForWindow(qWindow);
	if (surface == nullptr)
		Logger::error(RenderInstance::vkOutput, u8"无法创建VkSurfaceKHR");
	return surface;
}

VkSwapchainKHR helix::vulkan::SwapChain::createSwapChain()
{
	auto devices = RenderInstance::getDevices();

	VkSwapchainKHR swapChain{};

	return swapChain;
}
