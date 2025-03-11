#include <string>
#include <fast_io.h>
#include <Essence/Vulkan/Device.hpp>

#if !NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData);

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                          const VkAllocationCallbacks* pAllocator);
#endif

std::vector<helix::Ref<essence::Device>> essence::vulkan::Device::makeDevices()
{
	auto instance = VulkanInstance::getInstance().instance;
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	if (physicalDeviceCount == 0)
	{
		helix::Logger::warning(vkOutput, u8"无可用 Device");
		return {};
	}
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	std::vector<helix::Ref<essence::Device>> result(physicalDeviceCount);
	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
		auto physical = physicalDevices[i];
		helix::Ref device = new Device;
		device->physicalDevice = physical;

		VkPhysicalDeviceProperties physicalProperties;
		VkPhysicalDeviceFeatures physicalFeatures;
		vkGetPhysicalDeviceProperties(physical, &physicalProperties);
		vkGetPhysicalDeviceFeatures(physical, &physicalFeatures);
		device->setName(reinterpret_cast<const char8_t*>(physicalProperties.deviceName));
		result[i] = device;
	}

	return result;
}

essence::vulkan::Device::VulkanInstance::VulkanInstance()
{
	VkApplicationInfo applicationCI{};
	applicationCI.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationCI.pApplicationName = "Helix Application";
	applicationCI.pEngineName = "Helix Engine";


	VkInstanceCreateInfo instanceCI{};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pApplicationInfo = &applicationCI;

	//准确讲是WSI必需的拓展，后续再把WSI做成组件的形式
	bool isSupportedVulkanSurface = false;

	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (const auto& extension: extensions) //感谢GLFW的源码帮助~
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
	{
		//后续做成组件时，如果不是必需，则移除拓展而非log error
		helix::Logger::error(vkOutput, u8"找不到WSI需要的Vulkan 拓展");
	}

	std::vector neededExtensions = {
			"VK_KHR_surface",
			wsiPlatformExtension.c_str(),
	};

#if !NDEBUG

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	std::u8string allLayerName;
	for (const auto& layer: layers)
	{
		allLayerName += '\t';
		allLayerName += reinterpret_cast<const char8_t*>(layer.layerName);
		allLayerName += '\n';
	}
	helix::Logger::info(vkOutput, u8"可用的 Vulkan Layers：\n ", allLayerName);

	std::vector optionalLayers =
	{
			"VK_LAYER_KHRONOS_validation",
#if !(defined(ANDROID) || defined(_ANDROID_))
			"VK_LAYER_LUNARG_standard_validation"
#else
			"VK_LAYER_GOOGLE_threading"
			"VK_LAYER_LUNARG_parameter_validation"
			"VK_LAYER_LUNARG_object_tracker"
			"VK_LAYER_LUNARG_core_validation"
			"VK_LAYER_LUNARG_image"
			"VK_LAYER_LUNARG_swapchain"
			"VK_LAYER_GOOGLE_unique_objects");
#endif
	};

	for (auto optionalLayer = optionalLayers.begin(); optionalLayer != optionalLayers.end();)
	{
		bool isFound = false;
		for (const auto& layer: layers)
			if (layer.layerName == std::string(*optionalLayer))
				isFound = true;

		if (!isFound)
		{
			helix::Logger::info(vkOutput, u8"找不到Layer：",
			                    std::u8string_view(reinterpret_cast<const char8_t*>(*optionalLayer)), u8"，已将其移除");
			optionalLayer = optionalLayers.erase(optionalLayer);
		} else
		{
			++optionalLayer;
		}
	}

	if (!optionalLayers.empty()) //有layer则代表debug util存在
		neededExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	instanceCI.enabledLayerCount = static_cast<uint32_t>(optionalLayers.size());
	instanceCI.ppEnabledLayerNames = optionalLayers.data();
#endif

	instanceCI.enabledExtensionCount = static_cast<uint32_t>(neededExtensions.size());
	instanceCI.ppEnabledExtensionNames = neededExtensions.data();

	resultProcess(vkCreateInstance(&instanceCI, nullptr, &instance), u8"VkInstance 创建失败");

#if !NDEBUG
	// Debug Utils Messenger
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI = {};
	debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCI.pfnUserCallback = debugCallback;

	resultProcess(createDebugUtilsMessengerEXT(instance, &debugUtilsMessengerCI, nullptr, &debugUtilsMessenger),
	              u8"Debug Utils Messenger 创建失败");
#endif
}

essence::vulkan::Device::VulkanInstance::~VulkanInstance()
{
#if !NDEBUG
	destroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
#endif

	vkDestroyInstance(instance, nullptr);
}

void essence::vulkan::Device::resultProcess(const VkResult result, const std::u8string_view errorMsg)
{
	if (result != VK_SUCCESS)
	{
		helix::Logger::error(vkOutput, errorMsg);
	}
}

helix::Logger::Output essence::vulkan::Device::vkOutput = [](helix::MessageLevel level, std::u8string_view content)
{
	println(fast_io::u8out(), fast_io::u8concat_std(u8"Essence:[Vulkan ",
	                                                helix::Logger::getMessageLevelName(level),
	                                                u8"] ",
	                                                content));
};

#if !NDEBUG
// ReSharper disable once CppDFAConstantFunctionResult
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
	auto level = helix::MessageLevel::Info;
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			level = helix::MessageLevel::Info;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			level = helix::MessageLevel::Warning;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			level = helix::MessageLevel::Error;
			break;
		default:
			return VK_FALSE; //无关紧要的消息
	}
	helix::Logger::print(essence::vulkan::Device::vkOutput, level, u8"<Validation Layer> "
	                     //, reinterpret_cast<const char8_t* const>(pCallbackData->pMessage)
			);

	return VK_FALSE;
}

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			instance, "vkCreateDebugUtilsMessengerEXT"));
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}


static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                          // ReSharper disable once CppDFAConstantParameter
                                          const VkAllocationCallbacks* pAllocator)
{
	auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
		vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}
#endif
