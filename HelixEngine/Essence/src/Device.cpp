#include <Essence/Device.hpp>
#include <Essence/Vulkan/Device.hpp>

essence::Device::Instance::Instance()
{
	switch (api)
	{
		case GraphicsApi::Vulkan:
			devices = vulkan::Device::makeDevices();
			break;
		default:
			break;
	}
}

void essence::Device::setGraphicsApi(const GraphicsApi api)
{
	Device::api = api;
}

essence::GraphicsApi essence::Device::graphicsApi()
{
	return api;
}
