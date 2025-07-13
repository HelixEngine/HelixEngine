#pragma once
#include <ktm/ktm.h>

namespace helix
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;
	template<typename T>
	concept Enum = std::is_enum_v<T>;

	template<Arithmetic T>
	class Vector2T;
	template<Arithmetic T>
	class Vector3T;

	template<Enum T>
	class Vector2E;
	template<Enum T>
	class Vector3E;
}

namespace ktm
{
	template<typename T>
	struct is_vector<helix::Vector2T<T>> : std::true_type
	{
	};

	template<typename T>
	struct is_floating_point_base<helix::Vector2T<T>> : std::true_type
	{
	};

	template<typename T>
	struct math_traits<helix::Vector2T<T>>
	{
		using base_type = T;
		static constexpr size_t len = 2;
	};

	template<typename T>
	struct is_vector<helix::Vector3T<T>> : std::true_type
	{
	};

	template<typename T>
	struct is_floating_point_base<helix::Vector3T<T>> : std::true_type
	{
	};

	template<typename T>
	struct math_traits<helix::Vector3T<T>>
	{
		using base_type = T;
		static constexpr size_t len = 3;
	};
}

namespace helix
{
	template<Arithmetic T>
	class Vector3T
	{
	public:
		using KtmVec = ktm::vec<3, T>;
		T x{};
		T y{};
		T z{};

		Vector3T() = default;

		explicit Vector3T(KtmVec ktmVec) :
			x(ktmVec.x), y(ktmVec.y), z(ktmVec.z)
		{
		}

		Vector3T(T x, T y, T z):
			x(x), y(y), z(z)
		{
		}

		auto getLength() const
		{
			return ktm::length(ktmVecSelf());
		}

		Vector3T& normalize()
		{
			ktmVecSelf() = ktm::normalize(ktmVecSelf());
			return *this;
		}

		bool operator==(const Vector3T& vec3) const = default;

		static Vector3T normalize(const Vector3T& vector3)
		{
			return Vector3T{ktm::normalize(vector3.ktmVecSelf())};
		}

		explicit operator KtmVec() const
		{
			return KtmVec(x, y, z);
		}

		template<typename OtherT>
		explicit operator Vector3T<OtherT>() const
		{
			return Vector3T<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y), static_cast<OtherT>(z));
		}

		Vector2T<T>& vector2()
		{
			return *reinterpret_cast<Vector2T<T>*>(this);
		}

		const Vector2T<T>& vector2() const
		{
			return *reinterpret_cast<const Vector2T<T>*>(this);
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

	template<Enum T>
	class Vector3E
	{
	public:
		T x{};
		T y{};
		T z{};
		Vector3E() = default;

		Vector3E(T x, T y, T z):
			x(x), y(y), z(z)
		{
		}

		template<typename OtherT>
		explicit operator Vector3T<OtherT>() const
		{
			return Vector3T<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y), static_cast<OtherT>(z));
		}

		Vector2E<T>& vector2()
		{
			return *reinterpret_cast<Vector2E<T>*>(this);
		}

		const Vector2E<T>& vector2() const
		{
			return *reinterpret_cast<const Vector2E<T>*>(this);
		}

		bool operator==(const Vector3E& vec3) const = default;
	};

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

		auto getLength() const
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

		template<typename OtherT>
		explicit operator Vector2T<OtherT>() const
		{
			return Vector2T<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y));
		}

		Vector3T<T> vector3(T z = T{}) const
		{
			return Vector3T<T>(x, y, z);
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

	template<Enum T>
	class Vector2E
	{
	public:
		T x{};
		T y{};

		Vector2E() = default;

		Vector2E(T x, T y):
			x(x), y(y)
		{
		}

		template<typename OtherT>
		explicit operator Vector2T<OtherT>() const
		{
			return Vector2T<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y));
		}

		Vector3E<T> vector3(T z = T{}) const
		{
			return Vector3E<T>(x, y, z);
		}

		bool operator==(const Vector2E& vec2) const = default;
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

	using Vector3F = Vector3T<float>;
	using Vector3Double = Vector3T<double>;
	using Vector3I8 = Vector3T<int8_t>;
	using Vector3UI8 = Vector3T<uint8_t>;
	using Vector3I16 = Vector3T<int16_t>;
	using Vector3UI16 = Vector3T<uint16_t>;
	using Vector3I32 = Vector3T<int32_t>;
	using Vector3UI32 = Vector3T<uint32_t>;
	using Vector3I64 = Vector3T<int64_t>;
	using Vector3UI64 = Vector3T<uint64_t>;

	template<typename T>
	struct IsVector2
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct IsVector2<Vector2T<T>>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	struct IsVector2<Vector2E<T>>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	concept Vector2Type = IsVector2<T>::value;

	template<typename T>
	struct IsVector3
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct IsVector3<Vector3T<T>>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	struct IsVector3<Vector3E<T>>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	struct IsVector
	{
		static constexpr bool value = IsVector2<T>::value || IsVector3<T>::value;
	};

	template<typename T>
	concept Vector3Type = IsVector3<T>::value;

	template<typename T>
	concept VectorType = Vector2Type<T> || Vector3Type<T>;
}
