#include <HelixEngine/Render/Resource.hpp>
#include <HelixEngine/Util/Logger.hpp>

helix::Ref<helix::Bitmap> helix::Bitmap::load(const std::u8string& filePath, const Config& config)
{
	Ref bitmap = new Bitmap;
	bitmap->setName(u8"Bitmap:" + filePath);
	bitmap->innerLoad(filePath, config);
	return bitmap;
}

void helix::Bitmap::innerLoad(const std::u8string& filePath, const Config& config)
{
	image.load(reinterpret_cast<const char*>(filePath.data()));
	if (config.bitmapFormat != PixelFormat::Unknown)
	{
		auto sailPixelFormat = formatConvert(config.bitmapFormat);
		if (image.can_convert(sailPixelFormat))
		{
			image.convert(sailPixelFormat);
			format = config.bitmapFormat;
			return;
		}
		Logger::info(u8"加载的图像（", filePath, u8"）不支持Config指定的Bitmap PixelFormat，默认不执行格式转换");
	}

	format = formatConvert(image.pixel_format());
}

SailPixelFormat helix::Bitmap::formatConvert(const PixelFormat& format)
{

}

std::unordered_map<SailPixelFormat, helix::Bitmap::ConvertInfo<helix::PixelFormat>>
helix::Bitmap::fromSailPixelFormatMap = {
		{SAIL_PIXEL_FORMAT_UNKNOWN, {helix::PixelFormat::Unknown}},
		{SAIL_PIXEL_FORMAT_BPP32_RGBA, {helix::PixelFormat::RGBA8UNorm}},
		{SAIL_PIXEL_FORMAT_BPP64_RGBA, {helix::PixelFormat::RGBA16UNorm}},
		{SAIL_PIXEL_FORMAT_BPP128, {helix::PixelFormat::RGBA32Float}},
		{SAIL_PIXEL_FORMAT_BPP96, {helix::PixelFormat::RGB32Float}},
		{SAIL_PIXEL_FORMAT_BPP64, {helix::PixelFormat::RG32Float}},
		{SAIL_PIXEL_FORMAT_BPP32, {helix::PixelFormat::R32Float}},
		{SAIL_PIXEL_FORMAT_BPP32_BGRA, {helix::PixelFormat::BGRA8UNorm}},
		{SAIL_PIXEL_FORMAT_BPP24_BGR, {helix::PixelFormat::BGRX8UNorm, true}},
		{SAIL_PIXEL_FORMAT_BPP32_BGRX, {helix::PixelFormat::BGRX8UNorm}},
};

helix::PixelFormat helix::Bitmap::formatConvert(SailPixelFormat format)
{
	return PixelFormat::Unknown;
}
