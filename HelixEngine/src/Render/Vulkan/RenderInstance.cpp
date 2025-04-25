#include <HelixEngine/Render/Vulkan/RenderInstance.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <HelixEngine/Util/Singleton.hpp>
#include <HelixEngine/Render/Vulkan/DebugUtil.hpp>
#include <HelixEngine/Render/Vulkan/SwapChain.hpp>

helix::Logger::Output helix::vulkan::RenderInstance::vkOutput = [](MessageLevel level, std::u8string_view content)
{
	println(fast_io::u8out(), fast_io::u8concat_std(u8"HelixEngine:[Vulkan ",
	                                                Logger::getMessageLevelName(level),
	                                                u8"] ",
	                                                content));
};

void helix::vulkan::RenderInstance::resultProcess(VkResult result, std::u8string_view errMsg)
{
	if (result != VK_SUCCESS)
		Logger::error(vkOutput, errMsg);
}

VkInstance helix::vulkan::RenderInstance::createVkInstance()
{
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
#if HELIX_DEBUG
	{
		std::u8string allExtName;
		for (const auto& ext: instanceProperty.extensions)
		{
			allExtName += '\n';
			allExtName += '\t';
			allExtName += reinterpret_cast<const char8_t*>(ext.extensionName);
		}
		Logger::info(vkOutput, u8"可用的 Instance Extensions： (", instanceProperty.extensions.size(), u8"个)",
		             allExtName);
	}
#endif

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	instanceProperty.layers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, instanceProperty.layers.data());
#if HELIX_DEBUG
	{
		std::u8string allLayerName;
		for (const auto& layer: instanceProperty.layers)
		{
			allLayerName += '\n';
			allLayerName += '\t';
			allLayerName += reinterpret_cast<const char8_t*>(layer.layerName);
		}
		Logger::info(vkOutput, u8"可用的 Instance Layers： (", instanceProperty.layers.size(), u8"个)",
		             allLayerName);
	}
#endif

	//VK_KHR_get_physical_device_properties2
	{
		uint32_t vkVersion;
		vkEnumerateInstanceVersion(&vkVersion);
		auto it = std::ranges::find_if(instanceProperty.extensions,
		                               [](const VkExtensionProperties& extensionProperties)
		                               {
			                               return extensionProperties.extensionName ==
			                                      std::string_view("VK_KHR_get_physical_device_properties2");
		                               });
		if (vkVersion >= VK_MAKE_API_VERSION(0, 1, 1, 0))
		{
			isSupportProperties2 = true;
		}

		if (it != instanceProperty.extensions.end())
		{
			isSupportProperties2 = true;
			instanceProperty.enabledExtensions.push_back("VK_KHR_get_physical_device_properties2");
		}
	}

	if (queryFeature(RenderFeature::DebugUtil))
		DebugUtil::config(instanceProperty);

	SwapChain::config(instanceProperty);

	instanceCI.enabledLayerCount = static_cast<uint32_t>(instanceProperty.enabledLayers.size());
	instanceCI.ppEnabledLayerNames = instanceProperty.enabledLayers.data();

	instanceCI.enabledExtensionCount = static_cast<uint32_t>(instanceProperty.enabledExtensions.size());
	instanceCI.ppEnabledExtensionNames = instanceProperty.enabledExtensions.data();

	VkInstance instance;
	resultProcess(vkCreateInstance(&instanceCI, nullptr, &instance), u8"VkInstance 创建失败");
	return instance;
}

helix::vulkan::Device helix::vulkan::RenderInstance::createDevice()
{
	Device result;
	// uint32_t physicalDeviceCount;
	// auto instance = vkInstance.get();
	// vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	// if (physicalDeviceCount == 0)
	// {
	// 	Logger::warning(vkOutput, u8"无可用 Device");
	// 	return {};
	// }
	// std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	// vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	//
	// size_t bestScore = 0;
	// VkPhysicalDevice bestPhysicalDevice = nullptr;
	// for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	// {
	// 	size_t score = 0;
	// 	DeviceProperty deviceProperty;
	// 	auto physicalDevice = physicalDevices[i];
	//
	// 	VkPhysicalDeviceProperties physicalProperties;
	// 	VkPhysicalDeviceFeatures physicalFeatures;
	// 	vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
	// 	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);
	//
	// 	if (physicalProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	// 	{
	// 		score += 10;
	// 	} else if (physicalProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	// 	{
	// 		score += 5;
	// 	} else if (physicalProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
	// 	{
	// 		score += 3;
	// 	} else if (physicalProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
	// 	{
	// 		score += 1;
	// 	}
	//
	// 	uint32_t extCount;
	// 	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
	// 	deviceProperty.extensions.resize(extCount);
	// 	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount,
	// 	                                     deviceProperty.extensions.data());
	//
	// 	uint32_t layerCount;
	// 	vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);
	// 	deviceProperty.layers.resize(layerCount);
	// 	vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, deviceProperty.layers.data());
	//
	// 	//
	// 	// uint32_t queueFamilyCount = 0;
	// 	// vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	// 	// std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	// 	// vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
	// 	//
	// 	// std::vector<VkDeviceQueueCreateInfo> queueCIs(queueFamilyCount);
	// 	// std::vector<std::vector<float>> priorities(queueFamilyCount);
	// 	// for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; ++queueFamilyIndex)
	// 	// {
	// 	// 	auto& queueCI = queueCIs[queueFamilyIndex];
	// 	// 	auto& queueFamily = queueFamilies[queueFamilyIndex];
	// 	// 	queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	// 	// 	queueCI.queueFamilyIndex = queueFamilyIndex;
	// 	// 	queueCI.queueCount = queueFamily.queueCount;
	// 	// 	priorities[queueFamilyIndex].resize(queueFamily.queueCount);
	// 	// 	std::ranges::fill(priorities[queueFamilyIndex], 1.f);
	// 	// 	queueCI.pQueuePriorities = priorities[queueFamilyIndex].data();
	// 	// }
	// 	//
	// 	// VkDeviceCreateInfo deviceCI{};
	// 	// deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	// 	// deviceCI.queueCreateInfoCount = queueFamilyCount;
	// 	// deviceCI.pQueueCreateInfos = queueCIs.data();
	// 	//
	// 	// deviceCI.enabledExtensionCount = static_cast<uint32_t>(deviceProperty.enabledExtensions.size());
	// 	// deviceCI.ppEnabledExtensionNames = deviceProperty.enabledExtensions.data();
	// 	// deviceCI.enabledLayerCount = static_cast<uint32_t>(deviceProperty.enabledLayers.size());
	// 	// deviceCI.ppEnabledLayerNames = deviceProperty.enabledLayers.data();
	// 	//
	// 	// resultProcess(vkCreateDevice(physicalDevice, &deviceCI, nullptr, &device.logicDevice), u8"Logic Device创建失败");
	//
	// 	if (score > bestScore)
	// 	{
	// 		bestScore = score;
	// 		bestPhysicalDevice = physicalDevice;
	// 	}
	// }
	return result;
}

std::vector<helix::vulkan::Device> helix::vulkan::RenderInstance::createDevices()
{
	uint32_t physicalDeviceCount;
	auto instance = vkInstance.get();
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	if (physicalDeviceCount == 0)
	{
		Logger::warning(vkOutput, u8"无可用 Device");
		return {};
	}
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	std::vector<Device> result(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
		DeviceProperty deviceProperty;
		auto physicalDevice = physicalDevices[i];

		VkPhysicalDeviceProperties physicalProperties;
		VkPhysicalDeviceFeatures physicalFeatures;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);

		uint32_t extCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
		deviceProperty.extensions.resize(extCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount,
		                                     deviceProperty.extensions.data());

		uint32_t layerCount;
		vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);
		deviceProperty.layers.resize(layerCount);
		vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, deviceProperty.layers.data());

		if (queryFeature(RenderFeature::DebugUtil))
			DebugUtil::config(deviceProperty);
		SwapChain::config(deviceProperty);


		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		std::vector<VkDeviceQueueCreateInfo> queueCIs(queueFamilyCount);
		std::vector<std::vector<float>> priorities(queueFamilyCount);
		for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; ++queueFamilyIndex)
		{
			auto& queueCI = queueCIs[queueFamilyIndex];
			auto& queueFamily = queueFamilies[queueFamilyIndex];
			queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCI.queueFamilyIndex = queueFamilyIndex;
			queueCI.queueCount = queueFamily.queueCount;
			priorities[queueFamilyIndex].resize(queueFamily.queueCount);
			std::ranges::fill(priorities[queueFamilyIndex], 1.f);
			queueCI.pQueuePriorities = priorities[queueFamilyIndex].data();
		}

		VkDeviceCreateInfo deviceCI{};
		deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCI.queueCreateInfoCount = queueFamilyCount;
		deviceCI.pQueueCreateInfos = queueCIs.data();

		deviceCI.enabledExtensionCount = static_cast<uint32_t>(deviceProperty.enabledExtensions.size());
		deviceCI.ppEnabledExtensionNames = deviceProperty.enabledExtensions.data();
		deviceCI.enabledLayerCount = static_cast<uint32_t>(deviceProperty.enabledLayers.size());
		deviceCI.ppEnabledLayerNames = deviceProperty.enabledLayers.data();

		resultProcess(vkCreateDevice(physicalDevice, &deviceCI, nullptr, &result[i].logicDevice), u8"Logic Device创建失败");
		result[i].features = physicalFeatures;
		result[i].properties = physicalProperties;
		result[i].physicalDevice = physicalDevice;
		result[i].renderFeatures = deviceProperty.renderFeatures;
	}
	return result;
}

helix::vulkan::Device helix::vulkan::RenderInstance::pickBestDevice(BitOption<RenderFeature> requiredFeature)
{
	auto& devices = RenderInstance::devices.get();
	if (devices.size() == 1)
		return devices[0];
	size_t bestScore = 0;
	Device bestDevice{};
	for (const auto& device: devices)
	{
		size_t score = 0;
		if (!device.renderFeatures.isContain(requiredFeature))
			continue;

		switch (device.properties.deviceType)
		{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				score += 10;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				score += 5;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				score += 3;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				score += 1;
				break;
			default: ;
		}

		score += device.renderFeatures.getEnabledItemSize();

		if (score > bestScore)
		{
			bestScore = score;
			bestDevice = device;
		}
	}
	return bestDevice;
}

VkInstance helix::vulkan::RenderInstance::getVkInstance()
{
	return vkInstance.get();
}

const std::vector<helix::vulkan::Device>& helix::vulkan::RenderInstance::getDevices()
{
	return devices.get();
}

std::function<void(helix::RenderThreadInstance)> helix::vulkan::RenderInstance::getThreadFunc()
{
	return threadFunc;
}

void helix::vulkan::RenderInstance::threadFunc(RenderThreadInstance threadInstance)
{
	auto instance = vkInstance.get();
	auto device = pickBestDevice(Renderer::getRenderFeature());

	auto renderer = threadInstance.renderer;
	auto renderQueue = renderer->getRenderQueue();
	renderQueue->setBackBuffer(new RenderCommandBuffer);
	while (true)
	{
		auto backBuffer = swapBackBuffer(renderQueue);
		if (backBuffer->empty())
			break;

		std::unique_ptr<RenderCommand> cmd{};
		do
		{
			cmd = backBuffer->acquireCommand();
			switch (cmd->type)
			{
				case RenderCommand::Type::Begin:
					Logger::info(vkOutput, u8"Begin");
				//...
					break;
				case RenderCommand::Type::End:
					Logger::info(vkOutput, u8"End");
				//...
					break;
				default:
					break;
			}
		} while (cmd->type != RenderCommand::Type::End);
	}
}
