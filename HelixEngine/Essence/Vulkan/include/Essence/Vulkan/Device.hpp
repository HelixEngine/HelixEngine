#pragma once
#include <Essence/Device.hpp>
#include <vulkan/vulkan.h>
#include <HelixEngine/Util/Logger.hpp>

namespace essence::vulkan
{
	class Device final : public essence::Device
	{
	public: //测试的public
		friend class essence::Device;
		static std::vector<helix::Ref<essence::Device>> makeDevices();
		static VkInstance getVkInstance();

		struct VulkanInstance : helix::Singleton<VulkanInstance>
		{
			VulkanInstance();
			~VulkanInstance();
			VkInstance instance;
		};

		static void resultProcess(VkResult result, std::u8string_view errorMsg);
		static helix::Logger::Output vkOutput;

		struct InstanceProperty
		{
			std::vector<VkExtensionProperties> extensions;
			std::vector<const char*> enabledExtensions;
#if !NDEBUG
			std::vector<VkLayerProperties> layers;
			std::vector<const char*> enabledLayers;
#endif
			std::vector<helix::Ref<Component>> globalComponents;
		};

		VkPhysicalDevice physicalDevice{};
		VkDevice logicDevice{};
	};
}
