#pragma once
#include <HelixEngine/Util/Ref.hpp>

namespace essence
{
	class Device;
}

namespace essence
{
	class Component : public helix::Object
	{
	public:
		static constexpr std::u8string_view componentName = u8"Component";
		virtual std::u8string_view getComponentName() const = 0;
	};

	class DeviceComponent : public Component
	{
	public:
		static constexpr std::u8string_view componentName = u8"DeviceComponent";

		Device* getDevice() const
		{
			return device;
		}
	protected:
		Device* device = nullptr;
	};

	template<typename ComponentType>
	concept BasedComponent = helix::BasedObject<ComponentType, Component> && requires
	{
		{ ComponentType::componentName } -> std::convertible_to<std::u8string_view>;
	};

	namespace component
	{
		class Loader : public helix::Object
		{
		};
	}
}
