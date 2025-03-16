#pragma once
#include <HelixEngine/Util/Object.hpp>

namespace essence
{
	class Queue : public helix::Object
	{
		friend class Device;
	protected:
		Queue() = default;
	public:
		enum class Type
		{
			Graphics,
			Compute,
			Transfer,
		};
	};
}
