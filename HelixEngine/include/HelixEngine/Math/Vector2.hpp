#pragma once
#include <ktm/ktm.h>

namespace helix
{
	template<typename T>
	using Vector2T = ktm::vec<2, T>;
	using Vector2F = ktm::fvec2;
	using Vector2I32 = ktm::vec<2, int32_t>;
	using Vector2UI32 = ktm::vec<2, uint32_t>;
	using Vector2I16 = ktm::vec<2, int16_t>;
	using Vector2UI16 = ktm::vec<2, uint16_t>;
}
