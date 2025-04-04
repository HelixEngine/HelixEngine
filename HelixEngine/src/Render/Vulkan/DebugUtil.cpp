#include <HelixEngine/Render/Vulkan/DebugUtil.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <HelixEngine/Render/Vulkan/RenderInstance.hpp>

// ReSharper disable once CppDFAConstantFunctionResult
VkBool32 helix::vulkan::DebugUtil::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                 void* pUserData)
{
	auto level = MessageLevel::Info;
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			level = MessageLevel::Info;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			level = MessageLevel::Warning;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			level = MessageLevel::Error;
			break;
		default:
			return VK_FALSE; //无关紧要的消息
	}
	Logger::print(RenderInstance::vkOutput, level, u8"<Validation Layer> ",
	              std::u8string_view(reinterpret_cast<const char8_t* const>(pCallbackData->pMessage))
			);

	return VK_FALSE;
}

VkResult helix::vulkan::DebugUtil::createDebugUtilsMessengerEXT(VkInstance instance,
                                                                const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                                const VkAllocationCallbacks* pAllocator,
                                                                VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			instance, "vkCreateDebugUtilsMessengerEXT"));
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void helix::vulkan::DebugUtil::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                                             const VkAllocationCallbacks* pAllocator)
{
	auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			instance, "vkDestroyDebugUtilsMessengerEXT"));
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

void helix::vulkan::DebugUtil::config(InstanceProperty& instanceProperty)
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
			Logger::info(RenderInstance::vkOutput, u8"找不到Layer：",
			             std::u8string_view(reinterpret_cast<const char8_t*>(*optionalLayer)), u8"，已将其移除");
			optionalLayer = optionalLayers.erase(optionalLayer);
		} else
		{
			++optionalLayer;
		}
	}

	if (!optionalLayers.empty()) //有layer则代表debug util存在
	{
		instanceProperty.enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		isSupportValidationLayer = true;
	}
	instanceProperty.enabledLayers.insert(
			instanceProperty.enabledLayers.end(),
			optionalLayers.begin(),
			optionalLayers.end());
}

void helix::vulkan::DebugUtil::config(DeviceProperty& deviceProperty)
{
	if (isSupportValidationLayer)
	{
		deviceProperty.enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
		deviceProperty.renderFeatures.setItem(RenderFeature::DebugUtil, true);
	}
}
