#include <Essence/Device.hpp>
#include <Essence/Vulkan/Device.hpp>

essence::Device::Instance::Instance()
{
	switch (graphicsApi)
	{
		case GraphicsApi::Vulkan:
			devices = vulkan::Device::makeDevices();
			break;
		default:
			break;
	}
}

void essence::Device::setGraphicsApi(const GraphicsApi graphicsApi)
{
	Device::graphicsApi = graphicsApi;
}

essence::GraphicsApi essence::Device::getGraphicsApi()
{
	return graphicsApi;
}

const std::vector<helix::Ref<essence::Device>>& essence::Device::getDevice()
{
	return Instance::getInstance().devices;
}

void essence::Device::setComponentLoader(std::vector<helix::Ref<component::Loader>> loaders)
{
	Device::loaders = std::move(loaders);
}

const std::vector<helix::Ref<essence::Component>>& essence::Device::getAllGlobalComponent()
{
	Instance::getInstance();
	return globalComponents;
}

const std::vector<helix::Ref<essence::Component>>& essence::Device::getAllComponent()
{
	return components;
}
