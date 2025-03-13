#pragma once
#include <Essence/Component/DebugUtil.hpp>
#include <Essence/Vulkan/Component/Loader.hpp>

namespace essence::vulkan::component
{
	class DebugUtil final : public essence::component::DebugUtil
	{
		friend class essence::component::DebugUtil;
		class Loader;
		VkDebugUtilsMessengerEXT debugUtilsMessenger;
	public:
		~DebugUtil() override;
		void setMessageOutput(bool isEnabled = true) override;
	};

	class DebugUtil::Loader final : public component::Loader
	{
	public:
		void load(Device::InstanceProperty& instanceProperty) override;
	};
}
