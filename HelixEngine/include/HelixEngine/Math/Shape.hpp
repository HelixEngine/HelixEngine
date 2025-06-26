#pragma once
#include <HelixEngine/Math/Vector2.hpp>

namespace helix
{
	template<Vector2Type PositionType, Vector2Type SizeType>
	struct Rect
	{
		PositionType position;
		SizeType size;
	};
}
