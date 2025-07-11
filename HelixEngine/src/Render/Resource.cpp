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
		if (!image.can_convert(sailPixelFormat))
		{
			Logger::info(u8"加载的图像（", filePath, u8"）不支持Config指定的Bitmap PixelFormat，默认不执行格式转换");
			//...
		}
		image.convert(sailPixelFormat);
		//...
	}
}

SailPixelFormat helix::Bitmap::formatConvert(PixelFormat format)
{
	//...
	return SAIL_PIXEL_FORMAT_UNKNOWN;
}
