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

const std::vector<helix::Ref<essence::Device>>& essence::Device::getDevices()
{
	return Instance::getInstance().devices;
}

void essence::Device::setComponentLoaders(std::vector<helix::Ref<component::Loader>> loaders)
{
	Device::loaders = std::move(loaders);
}

std::vector<helix::Ref<essence::Component>> essence::Device::getGlobalComponents()
{
	Instance::getInstance();
	return globalComponents;
}
