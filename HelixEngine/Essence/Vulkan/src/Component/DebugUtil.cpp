#include <Essence/Vulkan/Component/DebugUtil.hpp>


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData);

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                          const VkAllocationCallbacks* pAllocator);


essence::vulkan::component::DebugUtil::~DebugUtil()
{
	setMessageOutput(false);
}

void essence::vulkan::component::DebugUtil::setMessageOutput(bool isEnabled)
{
	if (!isEnabled)
	{
		if (!debugUtilsMessenger)
			return;
		destroyDebugUtilsMessengerEXT(Device::getVkInstance(), debugUtilsMessenger, nullptr);
		debugUtilsMessenger = nullptr;
		return;
	}
	// Debug Utils Messenger
	//后续这些选项需要让用户指定，可以有默认选项
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI = {};
	debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCI.pfnUserCallback = debugCallback;

	Device::resultProcess(
			createDebugUtilsMessengerEXT(Device::getVkInstance(), &debugUtilsMessengerCI, nullptr,
			                             &debugUtilsMessenger),
			u8"Debug Utils Messenger 创建失败");
}

void essence::vulkan::component::DebugUtil::Loader::load(InstanceProperty& instanceProperty)
{
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
		//需要用std find优化一下
		bool isFound = false;
		for (const auto& layer: instanceProperty.layers)
			if (layer.layerName == std::string(*optionalLayer))
				isFound = true;

		if (!isFound)
		{
			helix::Logger::info(Device::vkOutput, u8"找不到Layer：",
			                    std::u8string_view(reinterpret_cast<const char8_t*>(*optionalLayer)), u8"，已将其移除");
			optionalLayer = optionalLayers.erase(optionalLayer);
		} else
		{
			++optionalLayer;
		}
	}

	if (!optionalLayers.empty()) //有layer则代表debug util存在
		instanceProperty.enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	instanceProperty.enabledLayers.insert(
			instanceProperty.enabledLayers.end(),
			optionalLayers.begin(),
			optionalLayers.end());
	instanceProperty.globalComponents.push_back(new DebugUtil);
}

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
	                     , std::u8string_view(reinterpret_cast<const char8_t* const>(pCallbackData->pMessage))
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
