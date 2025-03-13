#include <Essence/Vulkan/Component/DebugUtil.hpp>
#include <Essence/Component/DebugUtil.hpp>
#include <Essence/Device.hpp>

helix::Ref<essence::component::Loader> essence::component::DebugUtil::getLoader()
{
	switch (Device::getGraphicsApi())
	{
		case GraphicsApi::Vulkan:
			return new vulkan::component::DebugUtil::Loader;
			break;
		default: ;
	}
	return nullptr;
}
