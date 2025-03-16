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
			Graphics = 0b1,
			Compute = 0b10,
			Transfer = 0b100,
			Present = 0b1000,
		};
	};
}
