#pragma once
#include <Essence/Component/Component.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace essence::component
{
	class DebugUtil : public Component
	{
	public:
		static helix::Ref<Loader> getLoader();
		virtual void setMessageOutput(bool isEnabled = true) = 0;
	};
}
