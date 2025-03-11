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

		struct VulkanInstance : helix::Singleton<VulkanInstance>
		{
			VulkanInstance();
			~VulkanInstance();
			VkInstance instance;
			VkDebugUtilsMessengerEXT debugUtilsMessenger;
			//后续这个要挪到WSI组件里
			std::string wsiPlatformExtension;
		};

		static void resultProcess(VkResult result, std::u8string_view errorMsg);
		static helix::Logger::Output vkOutput;

		VkPhysicalDevice physicalDevice{};
		VkDevice logicDevice{};
	};
}
