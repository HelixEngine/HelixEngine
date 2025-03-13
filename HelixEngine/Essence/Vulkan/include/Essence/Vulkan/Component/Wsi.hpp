#pragma once
#include <Essence/Component/Wsi.hpp>
#include <Essence/Vulkan/Component/Loader.hpp>

#include "Essence/Vulkan/Device.hpp"

namespace essence::vulkan::component
{
	class Wsi final : public essence::component::Wsi
	{
	public:
		class Loader;
	private:
		std::string platformExtensionName;
	};

	class Wsi::Loader final : public component::Loader
	{
		void load(Device::InstanceProperty& instanceProperty) override;
		helix::Ref<Component> load(VkPhysicalDevice physicalDevice) override;

		Wsi* wsi = nullptr;
	};
}
