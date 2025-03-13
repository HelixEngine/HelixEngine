#include <Essence/Component/Wsi.hpp>
#include <Essence/Vulkan/Component/Wsi.hpp>
#include <Essence/Device.hpp>

helix::Ref<essence::component::Loader> essence::component::Wsi::getLoader()
{
	helix::Ref<Loader> loader;
	switch (Device::getGraphicsApi())
	{
		case GraphicsApi::Vulkan:
			loader = new vulkan::component::Wsi::Loader;
			break;
		default: ;
	}
	return loader;
}
