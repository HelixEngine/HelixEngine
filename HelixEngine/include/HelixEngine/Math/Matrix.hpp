#pragma once
#include <ktm/type_mat.h>
#include <HelixEngine/Math/Vector.hpp>

namespace helix
{
	template<typename T, size_t Row, size_t Col>
	class MatrixT
	{
	public:
		using KtmMat = ktm::mat<Row,Col,T>;
		using RawData = std::array<ktm::vec<Col,T>,Row>;
		using ValueType = T;
		static constexpr size_t row = Row;
		static constexpr size_t col = Col;

		MatrixT() = default;
		explicit MatrixT(const KtmMat& mat)
		{
			ktmMatSelf() = mat;
		}

		explicit MatrixT(RawData data) : data(std::move(data)) {}

		explicit operator KtmMat()
		{
			return ktmMatSelf();
		}

		RawData& getRawData()
		{
			return data;
		}

		const RawData& getRawData() const
		{
			return data;
		}

		MatrixT operator+(const MatrixT& mat)
		{
			return MatrixT(ktmMatSelf() + mat.ktmMatSelf());
		}

		MatrixT operator-(const MatrixT& mat)
		{
			return MatrixT(ktmMatSelf() - mat.ktmMatSelf());
		}

		template<size_t nCol>
		MatrixT<T,Row,nCol> operator*(const MatrixT<T,Col,nCol>& mat)
		{
			return MatrixT<T,Row,nCol>(ktmMatSelf() * mat.ktmMatSelf());
		}

		MatrixT operator*(T k)
		{
			return MatrixT(ktmMatSelf() * k);
		}

		friend MatrixT operator*(T k, const MatrixT& mat)
		{
			return MatrixT(k * mat.ktmMatSelf());
		}

		MatrixT operator/(T k)
		{
			return MatrixT(ktmMatSelf() / k);
		}

		MatrixT& operator+=(const MatrixT& mat)
		{
			return *this = *this + mat;
		}

		MatrixT& operator-=(const MatrixT& mat)
		{
			return *this = *this - mat;
		}

		MatrixT& operator*=(const MatrixT<T,Col,Col>& mat)
		{
			return *this = *this * mat;
		}

		MatrixT operator*=(T k)
		{
			return *this = *this * k;
		}

		VectorT<T,Col> operator*(const VectorT<T, Row>& v) noexcept
		{
			return VectorT<T,Col>(ktmMatSelf() * static_cast<VectorT<T, Row>::KtmVec>(v));
		}

		friend VectorT<T,Row> operator*(const VectorT<T,Col>& v, const MatrixT& m) noexcept
		{
			return VectorT<T,Row>(static_cast<VectorT<T, Col>::KtmVec>(v) * m.ktmMatSelf());
		}

		MatrixT operator/=(T k)
		{
			return *this = *this / k;
		}

		static MatrixT zero()
		{
			return MatrixT();
		}

		static MatrixT identity() requires (Row == Col)
		{
			RawData data = {};
			identityFill<Row - 1,Col - 1>(data);
			return MatrixT(std::move(data));
		}
	private:

		template<size_t nRow, size_t nCol>
		static void identityFill(RawData& data)
		{
			data[nRow].to_array()[nCol] = 1;
			if constexpr (nRow != 0)
			{
				identityFill<nRow - 1,nCol - 1>(data);
			}
		}

		auto& ktmMatSelf()
		{
			return *reinterpret_cast<KtmMat*>(this);
		}

		const auto& ktmMatSelf() const
		{
			return *reinterpret_cast<const KtmMat*>(this);
		}
		RawData data;
	};

	template<typename T>
	using Matrix2T = MatrixT<T,2,2>;
	template<typename T>
	using Matrix3T = MatrixT<T,3,3>;
	template<typename T>
	using Matrix4T = MatrixT<T,4,4>;
	template<typename T>
	using Matrix2x3T = MatrixT<T,2,3>;
	template<typename T>
	using Matrix2x4T = MatrixT<T,2,4>;
	template<typename T>
	using Matrix3x2T = MatrixT<T,3,2>;
	template<typename T>
	using Matrix3x4T = MatrixT<T,3,4>;
	template<typename T>
	using Matrix4x2T = MatrixT<T,4,2>;
	template<typename T>
	using Matrix4x3T = MatrixT<T,4,3>;

	using Matrix2F = MatrixT<float,2,2>;
	using Matrix3F = MatrixT<float,3,3>;
	using Matrix4F = MatrixT<float,4,4>;
	using Matrix2x3F = MatrixT<float,2,3>;
	using Matrix2x4F = MatrixT<float,2,4>;
	using Matrix3x2F = MatrixT<float,3,2>;
	using Matrix3x4F = MatrixT<float,3,4>;
	using Matrix4x2F = MatrixT<float,4,2>;
	using Matrix4x3F = MatrixT<float,4,3>;

	using Matrix2Double = MatrixT<double,2,2>;
	using Matrix3Double = MatrixT<double,3,3>;
	using Matrix4Double = MatrixT<double,4,4>;
	using Matrix2x3Double = MatrixT<double,2,3>;
	using Matrix2x4Double = MatrixT<double,2,4>;
	using Matrix3x2Double = MatrixT<double,3,2>;
	using Matrix3x4Double = MatrixT<double,3,4>;
	using Matrix4x2Double = MatrixT<double,4,2>;
	using Matrix4x3Double = MatrixT<double,4,3>;

	using Matrix2I8 = MatrixT<int8_t,2,2>;
	using Matrix3I8 = MatrixT<int8_t,3,3>;
	using Matrix4I8 = MatrixT<int8_t,4,4>;
	using Matrix2x3I8 = MatrixT<int8_t,2,3>;
	using Matrix2x4I8 = MatrixT<int8_t,2,4>;
	using Matrix3x2I8 = MatrixT<int8_t,3,2>;
	using Matrix3x4I8 = MatrixT<int8_t,3,4>;
	using Matrix4x2I8 = MatrixT<int8_t,4,2>;
	using Matrix4x3I8 = MatrixT<int8_t,4,3>;

	using Matrix2UI8 = MatrixT<uint8_t,2,2>;
	using Matrix3UI8 = MatrixT<uint8_t,3,3>;
	using Matrix4UI8 = MatrixT<uint8_t,4,4>;
	using Matrix2x3UI8 = MatrixT<uint8_t,2,3>;
	using Matrix2x4UI8 = MatrixT<uint8_t,2,4>;
	using Matrix3x2UI8 = MatrixT<uint8_t,3,2>;
	using Matrix3x4UI8 = MatrixT<uint8_t,3,4>;
	using Matrix4x2UI8 = MatrixT<uint8_t,4,2>;
	using Matrix4x3UI8 = MatrixT<uint8_t,4,3>;

	using Matrix2I16 = MatrixT<int16_t,2,2>;
	using Matrix3I16 = MatrixT<int16_t,3,3>;
	using Matrix4I16 = MatrixT<int16_t,4,4>;
	using Matrix2x3I16 = MatrixT<int16_t,2,3>;
	using Matrix2x4I16 = MatrixT<int16_t,2,4>;
	using Matrix3x2I16 = MatrixT<int16_t,3,2>;
	using Matrix3x4I16 = MatrixT<int16_t,3,4>;
	using Matrix4x2I16 = MatrixT<int16_t,4,2>;
	using Matrix4x3I16 = MatrixT<int16_t,4,3>;

	using Matrix2UI16 = MatrixT<uint16_t,2,2>;
	using Matrix3UI16 = MatrixT<uint16_t,3,3>;
	using Matrix4UI16 = MatrixT<uint16_t,4,4>;
	using Matrix2x3UI16 = MatrixT<uint16_t,2,3>;
	using Matrix2x4UI16 = MatrixT<uint16_t,2,4>;
	using Matrix3x2UI16 = MatrixT<uint16_t,3,2>;
	using Matrix3x4UI16 = MatrixT<uint16_t,3,4>;
	using Matrix4x2UI16 = MatrixT<uint16_t,4,2>;
	using Matrix4x3UI16 = MatrixT<uint16_t,4,3>;

	using Matrix2I32 = MatrixT<int32_t,2,2>;
	using Matrix3I32 = MatrixT<int32_t,3,3>;
	using Matrix4I32 = MatrixT<int32_t,4,4>;
	using Matrix2x3I32 = MatrixT<int32_t,2,3>;
	using Matrix2x4I32 = MatrixT<int32_t,2,4>;
	using Matrix3x2I32 = MatrixT<int32_t,3,2>;
	using Matrix3x4I32 = MatrixT<int32_t,3,4>;
	using Matrix4x2I32 = MatrixT<int32_t,4,2>;
	using Matrix4x3I32 = MatrixT<int32_t,4,3>;

	using Matrix2UI32 = MatrixT<uint32_t,2,2>;
	using Matrix3UI32 = MatrixT<uint32_t,3,3>;
	using Matrix4UI32 = MatrixT<uint32_t,4,4>;
	using Matrix2x3UI32 = MatrixT<uint32_t,2,3>;
	using Matrix2x4UI32 = MatrixT<uint32_t,2,4>;
	using Matrix3x2UI32 = MatrixT<uint32_t,3,2>;
	using Matrix3x4UI32 = MatrixT<uint32_t,3,4>;
	using Matrix4x2UI32 = MatrixT<uint32_t,4,2>;
	using Matrix4x3UI32 = MatrixT<uint32_t,4,3>;

	template<typename T>
	struct IsMatrix
	{
		static constexpr bool value = false;
	};
	template<typename T,size_t Row,size_t Col>
	struct IsMatrix<MatrixT<T,Row,Col>>
	{
		static constexpr bool value = true;
	};
}
