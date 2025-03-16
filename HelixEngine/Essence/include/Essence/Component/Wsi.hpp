#pragma once
#include <Essence/Component/Component.hpp>
#include <Essence/Component/SwapChain.hpp>

namespace essence::component
{
	class Wsi : public DeviceComponent
	{
	public:
		static constexpr std::u8string_view componentName = u8"Wsi";
		std::u8string_view getComponentName() const override { return componentName; }
		static helix::Ref<Loader> getLoader();
		virtual helix::Ref<SwapChain> makeSwapChain(SwapChain::Property property) = 0;
	};
}
