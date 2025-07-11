#pragma once
#include <array>

namespace helix
{
	struct PixelFormat
	{
		enum class ColorFormat
		{
			Unknown,
			RGBA,
			RGB,
			RG,
			R,
			BGRA,
			BGR,
		};

		enum class StorageType
		{
			Typeless,
			Float,
			UInt,
			SInt,
			UNorm,
			SNorm,
		};

		enum class ColorSpace
		{
			None,
			SRGB,
		};

		static constexpr uint8_t maxComponents = 4;

		ColorFormat colorFormat{};
		std::array<StorageType, maxComponents> storageType = {};
		ColorSpace colorSpace{};
		std::array<int32_t, maxComponents> bitWidth = {};

		enum class Value
		{
			Unknown, // Unknown Pixel Format
			RGBA8UNorm, //RGBA bit-width:8 UNorm
			RGBA8UNormSRGB, //RGBA bit-width:8 UNorm SRGB
			RGBA16UNorm, // RGBA bit-width:16 UNorm
			RGBA16Float, // RGBA bit-width:16 Float
			RGBA32Float, // RGBA bit-width:32 Float
			RGB32Float, // RGB bit-width:32 Float
			RG32Float, // RG bit-width:32 Float
			R32Float, // R bit-width:32 Float
		};

		using enum Value;

		PixelFormat() = default;

		explicit(false) PixelFormat(Value value);

		[[nodiscard]] bool isUnknown() const;
	};

	inline bool operator==(const PixelFormat& format1, const PixelFormat& format2)
	{
		if (format1.colorFormat == PixelFormat::ColorFormat::Unknown &&
		    format2.colorFormat == PixelFormat::ColorFormat::Unknown)
			return true;
		return format1.colorFormat == format2.colorFormat &&
		       format1.storageType == format2.storageType &&
		       format1.colorSpace == format2.colorSpace &&
		       format1.bitWidth == format2.bitWidth;
	}

}
