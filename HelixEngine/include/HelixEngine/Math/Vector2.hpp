#pragma once
#include <ktm/ktm.h>

namespace helix
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;
	template<Arithmetic T>
	class Vector2T;
}


template<helix::Arithmetic T>
struct ktm::is_vector<helix::Vector2T<T>> : std::true_type
{
};

namespace helix
{
	template<Arithmetic T>
	class Vector2T
	{
	public:
		T x;
		T y;

		auto length() const
		{
			return ktm::length(*this);
		}

		Vector2T& normalize()
		{
			return *this;
		}
	};

	using Vector2F = Vector2T<float>;
	using Vector2I32 = Vector2T<int32_t>;
	using Vector2UI32 = Vector2T<uint32_t>;
	using Vector2I16 = Vector2T<int16_t>;
	using Vector2UI16 = Vector2T<uint16_t>;
}
