#include <HelixEngine/Util/PixelFormat.hpp>

helix::PixelFormat::PixelFormat(Value value)
{
	switch (value)
	{
		case RGBA8UNormSRGB:
			colorSpace = ColorSpace::SRGB;
			[[fallthrough]];
		case RGBA8UNorm:
			colorFormat = ColorFormat::RGBA;
			storageType = {StorageType::UNorm, StorageType::UNorm, StorageType::UNorm, StorageType::UNorm};
			bitWidth = {8, 8, 8, 8};
			break;
		case RGBA16UNorm:
			colorFormat = ColorFormat::RGBA;
			storageType = {StorageType::UNorm, StorageType::UNorm, StorageType::UNorm, StorageType::UNorm};
			bitWidth = {16, 16, 16, 16};
			break;
		case RGBA16Float:
			colorFormat = ColorFormat::RGBA;
			storageType = {StorageType::Float, StorageType::Float, StorageType::Float, StorageType::Float};
			bitWidth = {16, 16, 16, 16};
			break;
		case RGBA32Float:
			colorFormat = ColorFormat::RGBA;
			storageType = {StorageType::Float, StorageType::Float, StorageType::Float, StorageType::Float};
			bitWidth = {32, 32, 32, 32};
			break;
		case RGB32Float:
			colorFormat = ColorFormat::RGB;
			storageType = {StorageType::Float, StorageType::Float, StorageType::Float};
			bitWidth = {32, 32, 32};
			break;
		case RG32Float:
			colorFormat = ColorFormat::RG;
			storageType = {StorageType::Float, StorageType::Float};
			bitWidth = {32, 32};
			break;
		case R32Float:
			colorFormat = ColorFormat::R;
			storageType = {StorageType::Float};
			bitWidth = {32};
			break;
		case BGRA8UNorm:
			colorFormat = ColorFormat::BGRA;
			storageType = {StorageType::UNorm, StorageType::UNorm, StorageType::UNorm, StorageType::UNorm};
			bitWidth = {8, 8, 8, 8};
			break;
		case BGRX8UNorm:
			colorFormat = ColorFormat::BGRX;
			storageType = {StorageType::UNorm, StorageType::UNorm, StorageType::UNorm};
			bitWidth = {8, 8, 8, 8};
			break;
		case Unknown: [[fallthrough]];
		default:
			colorFormat = ColorFormat::Unknown;
			break;
	}
}

bool helix::PixelFormat::isUnknown() const
{
	return colorFormat == ColorFormat::Unknown;
}
