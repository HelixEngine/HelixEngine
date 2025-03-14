#include <string>
#include <fast_io.h>
#include <Essence/Vulkan/Device.hpp>
#include <Essence/Vulkan/Component/Loader.hpp>

std::vector<helix::Ref<essence::Device>> essence::vulkan::Device::makeDevices()
{
	auto& loaders = *reinterpret_cast<std::vector<helix::Ref<component::Loader>>*>(&essence::Device::loaders);
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
		DeviceProperty deviceProperty;
		auto physicalDevice = physicalDevices[i];
		helix::Ref device = new Device;
		device->physicalDevice = physicalDevice;
		deviceProperty.device = device;

		VkPhysicalDeviceProperties physicalProperties;
		VkPhysicalDeviceFeatures physicalFeatures;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);

		uint32_t extCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
		deviceProperty.extensions.resize(extCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount,
		                                     deviceProperty.extensions.data());

#if !DEBUG
		{
			std::u8string allExtName;
			for (const auto& ext: deviceProperty.extensions)
			{
				allExtName += '\n';
				allExtName += '\t';
				allExtName += reinterpret_cast<const char8_t*>(ext.extensionName);
			}
			helix::Logger::info(vkOutput, u8"可用的 Device Extensions： (", deviceProperty.extensions.size(), u8"个)",
			                    allExtName);
		}
#endif

		device->setName(reinterpret_cast<const char8_t*>(physicalProperties.deviceName));
		result[i] = device;
		for (const auto& loader: loaders)
		{
			loader->load(deviceProperty);
		}
		device->components = std::move(deviceProperty.deviceComponents);

		//这块要交给Queue处理
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		VkDeviceCreateInfo deviceCI{};

	}

	essence::Device::loaders.clear();
	essence::Device::loaders.shrink_to_fit();

	return result;
}

VkInstance essence::vulkan::Device::getVkInstance()
{
	return VulkanInstance::getInstance().instance;
}

essence::vulkan::Device::VulkanInstance::VulkanInstance()
{
	auto& loaders = *reinterpret_cast<std::vector<helix::Ref<component::Loader>>*>(&essence::Device::loaders);
	InstanceProperty instanceProperty;

	VkApplicationInfo applicationCI{};
	applicationCI.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationCI.pApplicationName = "Helix Application";
	applicationCI.pEngineName = "Helix Engine";


	VkInstanceCreateInfo instanceCI{};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pApplicationInfo = &applicationCI;

	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	instanceProperty.extensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, instanceProperty.extensions.data());
#if !NDEBUG
	{
		std::u8string allExtName;
		for (const auto& ext: instanceProperty.extensions)
		{
			allExtName += '\n';
			allExtName += '\t';
			allExtName += reinterpret_cast<const char8_t*>(ext.extensionName);
		}
		helix::Logger::info(vkOutput, u8"可用的 Instance Extensions： (", instanceProperty.extensions.size(), u8"个)",
		                    allExtName);
	}
#endif

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	instanceProperty.layers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, instanceProperty.layers.data());
#if !NDEBUG
	{
		std::u8string allLayerName;
		for (const auto& layer: instanceProperty.layers)
		{
			allLayerName += '\n';
			allLayerName += '\t';
			allLayerName += reinterpret_cast<const char8_t*>(layer.layerName);
		}
		helix::Logger::info(vkOutput, u8"可用的 Layers： (", instanceProperty.layers.size(), u8"个)",
		                    allLayerName);
	}
#endif

	for (const auto& loader: loaders)
	{
		loader->load(instanceProperty);
	}
	globalComponents.insert(globalComponents.end(), instanceProperty.globalComponents.begin(),
	                        instanceProperty.globalComponents.end());

	instanceCI.enabledLayerCount = static_cast<uint32_t>(instanceProperty.enabledLayers.size());
	instanceCI.ppEnabledLayerNames = instanceProperty.enabledLayers.data();

	instanceCI.enabledExtensionCount = static_cast<uint32_t>(instanceProperty.enabledExtensions.size());
	instanceCI.ppEnabledExtensionNames = instanceProperty.enabledExtensions.data();

	resultProcess(vkCreateInstance(&instanceCI, nullptr, &instance), u8"VkInstance 创建失败");
}

essence::vulkan::Device::VulkanInstance::~VulkanInstance()
{
	globalComponents.clear();
	vkDestroyInstance(instance, nullptr);
}

void essence::vulkan::Device::resultProcess(const VkResult result, const std::u8string_view errorMsg)
{
	if (result != VK_SUCCESS)
	{
		helix::Logger::error(vkOutput, errorMsg);
	}
}

VkPhysicalDevice essence::vulkan::Device::getVkPhysicalDevice() const
{
	return physicalDevice;
}

VkDevice essence::vulkan::Device::getVkDevice() const
{
	return logicDevice;
}

helix::Logger::Output essence::vulkan::Device::vkOutput = [](helix::MessageLevel level, std::u8string_view content)
{
	println(fast_io::u8out(), fast_io::u8concat_std(u8"Essence:[Vulkan ",
	                                                helix::Logger::getMessageLevelName(level),
	                                                u8"] ",
	                                                content));
};
