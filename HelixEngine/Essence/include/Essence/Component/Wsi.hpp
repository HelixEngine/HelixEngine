#pragma once
#include <Essence/Component/Component.hpp>

namespace essence::component
{
	class Wsi : public Component
	{
	public:
		static helix::Ref<Loader> getLoader();
	};
}
