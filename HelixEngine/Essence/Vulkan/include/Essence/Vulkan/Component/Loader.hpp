#pragma once
#include <Essence/Component/Component.hpp>
#include <Essence/Vulkan/Device.hpp>

namespace essence::vulkan::component
{
	class Loader : public essence::component::Loader
	{
		friend class Device;
	protected:
		virtual void load(InstanceProperty& instanceProperty)
		{
		}

		virtual void load(DeviceProperty& deviceProperty)
		{
		}
	};
}
