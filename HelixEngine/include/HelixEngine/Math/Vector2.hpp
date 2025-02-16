#pragma once
#include <qsize.h>
#include <ktm/ktm.h>

namespace helix
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;
	template<Arithmetic T>
	class Vector2T;
}

namespace ktm
{
	template<helix::Arithmetic T>
	struct is_vector<helix::Vector2T<T>> : std::true_type
	{
	};

	template<helix::Arithmetic T>
	struct is_floating_point_base<helix::Vector2T<T>> : std::true_type
	{
	};

	template<helix::Arithmetic T>
	struct math_traits<helix::Vector2T<T>>
	{
		using base_type = T;
		static inline constexpr size_t len = 2;
	};
}

namespace helix
{
	template<Arithmetic T>
	class Vector2T
	{
	public:
		using KtmVec = ktm::vec<2, T>;
		T x{};
		T y{};

		Vector2T() = default;

		explicit Vector2T(KtmVec ktmVec) :
			x(ktmVec.x), y(ktmVec.y)
		{
		}

		Vector2T(T x, T y):
			x(x), y(y)
		{
		}

		auto length() const
		{
			return ktm::length(ktmVecSelf());
		}

		Vector2T& normalize()
		{
			ktmVecSelf() = ktm::normalize(ktmVecSelf());
			return *this;
		}

		bool operator==(const Vector2T& vec2) const = default;

		static Vector2T normalize(const Vector2T& vector2)
		{
			return Vector2T{ktm::normalize(vector2.ktmVecSelf())};
		}

		explicit operator KtmVec() const
		{
			return KtmVec(x, y);
		}

		explicit operator QSize() const
		{
			return QSize(x, y);
		}
	private:
		auto& ktmVecSelf()
		{
			return *reinterpret_cast<KtmVec*>(this);
		}

		const auto& ktmVecSelf() const
		{
			return *reinterpret_cast<const KtmVec*>(this);
		}
	};

	using Vector2F = Vector2T<float>;
	using Vector2Double = Vector2T<double>;
	using Vector2I8 = Vector2T<int8_t>;
	using Vector2UI8 = Vector2T<uint8_t>;
	using Vector2I16 = Vector2T<int16_t>;
	using Vector2UI16 = Vector2T<uint16_t>;
	using Vector2I32 = Vector2T<int32_t>;
	using Vector2UI32 = Vector2T<uint32_t>;
	using Vector2I64 = Vector2T<int64_t>;
	using Vector2UI64 = Vector2T<uint64_t>;

}
