#pragma once
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/DelayInit.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <vulkan/vulkan.h>

namespace helix::vulkan
{
	struct FeatureProperty
	{
		std::vector<VkExtensionProperties> extensions;
		std::vector<const char*> enabledExtensions;
		std::vector<VkLayerProperties> layers;
		std::vector<const char*> enabledLayers;
	};

	struct InstanceProperty : FeatureProperty
	{

	};

	struct Device
	{
		VkPhysicalDevice physicalDevice = nullptr;
		VkDevice logicDevice = nullptr;
		VkPhysicalDeviceProperties properties{};
		VkPhysicalDeviceFeatures features{};
		BitOption<RenderFeature> renderFeatures;
	};

	struct DeviceProperty : FeatureProperty
	{
		VkPhysicalDevice physicalDevice = nullptr;
		BitOption<RenderFeature> renderFeatures;
	};

	class RenderInstance final : public helix::RenderInstance
	{
		static void resultProcess(VkResult result, std::u8string_view errMsg);
		static VkInstance createVkInstance();
		static inline DelayInit<VkInstance> vkInstance{createVkInstance};
		static Device createDevice();
		static std::vector<Device> createDevices();
		static inline DelayInit<Device> device{createDevice};
		static inline DelayInit<std::vector<Device>> devices{createDevices};
		static inline bool isSupportProperties2 = false;
		static Device pickBestDevice(BitOption<RenderFeature> requiredFeature);
	public:
		static VkInstance getVkInstance();
		static const std::vector<Device>& getDevices();
		static Logger::Output vkOutput;
		void threadFunc(RenderThreadInstance threadInstance) override;
	};
}
