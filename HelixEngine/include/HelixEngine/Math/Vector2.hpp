#pragma once
#include <ktm/ktm.h>

namespace helix
{
	template<typename T>
	class Vector2T
	{
	public:
		T x;
		T y;
	};

	using Vector2F = Vector2T<float>;
	using Vector2I32 = Vector2T<int32_t>;
	using Vector2UI32 = Vector2T<uint32_t>;
	using Vector2I16 = Vector2T<int16_t>;
	using Vector2UI16 = Vector2T<uint16_t>;
}
