#pragma once
#include <Essence/Component/Component.hpp>
#include <Essence/Vulkan/Device.hpp>
#include <vulkan/vulkan.h>

namespace essence::vulkan::component
{
	class Loader : public essence::component::Loader
	{
		friend class Device;
	protected:
		virtual void load(Device::InstanceProperty& instanceProperty)
		{
		}

		virtual helix::Ref<Component> load(VkPhysicalDevice physicalDevice) { return nullptr; }
	};
}
