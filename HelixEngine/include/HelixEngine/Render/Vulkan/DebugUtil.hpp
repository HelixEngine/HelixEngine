#pragma once
#include <vulkan/vulkan.h>

#include "RenderInstance.hpp"

namespace helix::vulkan
{
	class DebugUtil final
	{
	public:
		DebugUtil() = delete;
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
		                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		                                                    void* pUserData);

		static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
		                                             const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		                                             const VkAllocationCallbacks* pAllocator,
		                                             VkDebugUtilsMessengerEXT* pCallback);
		static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
		                                          const VkAllocationCallbacks* pAllocator);
		static void config(InstanceProperty& instanceProperty);
		static void config(DeviceProperty& deviceProperty);
		static inline bool isSupportValidationLayer = true;
	};
}
