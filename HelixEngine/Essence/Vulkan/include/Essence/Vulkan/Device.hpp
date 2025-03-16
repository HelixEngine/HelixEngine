#pragma once
#include <Essence/Device.hpp>
#include <vulkan/vulkan.h>
#include <HelixEngine/Util/Logger.hpp>

namespace essence::vulkan
{
	class Device final : public essence::Device
	{
		friend class essence::Device;
	public:
		static std::vector<helix::Ref<essence::Device>> makeDevice();
		static VkInstance getVkInstance();
	private:
		struct VulkanInstance : helix::Singleton<VulkanInstance>
		{
			VulkanInstance();
			~VulkanInstance();
			VkInstance instance;
			bool isSupportProperties2 = false;
		};
	public:
		static void resultProcess(VkResult result, std::u8string_view errorMsg);
		static helix::Logger::Output vkOutput;
		~Device() override;
		VkPhysicalDevice getVkPhysicalDevice() const;
		VkDevice getVkDevice() const;
		helix::Ref<Queue> makeQueue(Queue::Type type) override;
	private:
		VkPhysicalDevice physicalDevice{};
		VkDevice logicDevice{};
	};

	struct InstanceProperty
	{
		std::vector<VkExtensionProperties> extensions;
		std::vector<const char*> enabledExtensions;
		std::vector<VkLayerProperties> layers;
		std::vector<const char*> enabledLayers;
		std::vector<helix::Ref<Component>> globalComponents;
	};

	class DeviceFeature2
	{
		friend class Device;
		void** ppNext = nullptr;
		std::vector<void*> features;
	public:
		~DeviceFeature2()
		{
			for (const auto feature: features)
			{
				free(feature);
			}
		}

		template<typename FeatureType> requires requires(FeatureType feature)
		{
			{ feature.pNext } -> std::same_as<void*>;
		}
		void addFeature(FeatureType&& feature)
		{
			auto node = malloc(sizeof(FeatureType));
			*static_cast<FeatureType*>(node) = std::forward<FeatureType>(feature);
			features.push_back(node);
			if (ppNext)
				*ppNext = node;

			ppNext = &static_cast<FeatureType*>(node)->pNext;
		}
	};

	struct DeviceProperty
	{
		Device* device = nullptr;
		VkPhysicalDeviceFeatures deviceFeatures{};
		DeviceFeature2 deviceFeature2{};
		std::vector<VkExtensionProperties> extensions;
		std::vector<const char*> enabledExtensions;
		std::vector<VkLayerProperties> layers;
		std::vector<const char*> enabledLayers;
		std::vector<helix::Ref<Component>> deviceComponents;
	};
}
