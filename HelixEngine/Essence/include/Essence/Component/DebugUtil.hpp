#pragma once
#include <Essence/Component/Component.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace essence::component
{
	class DebugUtil : public Component
	{
	public:
		static constexpr std::u8string_view componentName = u8"DebugUtil";

		std::u8string_view getComponentName() const override
		{
			return componentName;
		}

		static helix::Ref<Loader> getLoader();
		virtual void setMessageOutput(bool isEnabled = true) = 0;
	};
}
