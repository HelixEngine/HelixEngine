#include <HelixEngine/Render/Resource.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <sstream>

void helix::RenderResource::usable()
{
	if (isUsable())
		return;
	std::shared_lock lock(mtx);
	cv.wait(lock, [&]
	{
		return isUsable();
	});
}

void helix::RenderResource::notify()
{
	bIsUsable.store(true, std::memory_order_relaxed);
	std::shared_lock lock(mtx);
	cv.notify_all();
}

bool helix::RenderResource::isUsable() const
{
	return bIsUsable.load(std::memory_order_relaxed);
}

helix::MemoryBuffer::Type helix::MemoryBuffer::getType() const
{
	return type;
}

helix::MemoryBuffer::Usage helix::MemoryBuffer::getUsage() const
{
	return usage;
}

void helix::MemoryBuffer::setType(Type type)
{
	this->type = type;
}

void helix::MemoryBuffer::setUsage(Usage usage)
{
	this->usage = usage;
}

void helix::Shader::setUsage(Usage usage)
{
	this->usage = usage;
}

helix::Ref<helix::Bitmap> helix::Bitmap::load(const std::u8string& filePath, const Config& config)
{
	Ref bitmap = new Bitmap;
	bitmap->innerLoad(filePath, config);
	bitmap->notify();
	return bitmap;
}

bool helix::Bitmap::convert(const PixelFormat& dstFormat)
{
	if (!imageConvertFormat(image, formatConvert(dstFormat)))
		return false;

	format = dstFormat;
	return true;
}

const helix::PixelFormat& helix::Bitmap::getPixelFormat() const
{
	return format;
}

helix::Vector2UI32 helix::Bitmap::getSize() const
{
	return {image.width(), image.height()};
}

const sail::image& helix::Bitmap::getSailImage() const
{
	return image;
}

void helix::Bitmap::innerLoad(const std::u8string& filePath, const Config& config)
{
	setName(u8"Bitmap:" + filePath);
	image.load(reinterpret_cast<const char*>(filePath.data()));
	auto sailSrcFormat = image.pixel_format();
	auto srcConvertInfo = formatConvert(sailSrcFormat);
	if (config.bitmapFormat != PixelFormat::Unknown)
	{
		if (imageConvertFormat(image, formatConvert(config.bitmapFormat)))
		{
			format = config.bitmapFormat;
			return;
		}
		Logger::info(u8"加载的图像（", filePath, u8"）不支持Config指定的Bitmap PixelFormat，默认不执行格式转换");
	}

	if (srcConvertInfo.isNeedConvert)
		imageConvertFormat(image, srcConvertInfo.dstFormat);
	format = srcConvertInfo.srcFormat;
}

SailPixelFormat helix::Bitmap::formatConvert(const PixelFormat& format)
{
	if (format.isUnknown())
		return SAIL_PIXEL_FORMAT_UNKNOWN;
	if (format == PixelFormat::RGBA8UNorm)
		return SAIL_PIXEL_FORMAT_BPP32_RGBA;
	if (format == PixelFormat::RGBA16UNorm)
		return SAIL_PIXEL_FORMAT_BPP64_RGBA;
	if (format == PixelFormat::RGBA32Float)
		return SAIL_PIXEL_FORMAT_BPP128;
	if (format == PixelFormat::RGB32Float)
		return SAIL_PIXEL_FORMAT_BPP96;
	if (format == PixelFormat::RG32Float)
		return SAIL_PIXEL_FORMAT_BPP64;
	if (format == PixelFormat::R32Float)
		return SAIL_PIXEL_FORMAT_BPP32;
	if (format == PixelFormat::BGRA8UNorm)
		return SAIL_PIXEL_FORMAT_BPP32_BGRA;
	if (format == PixelFormat::BGRX8UNorm)
		return SAIL_PIXEL_FORMAT_BPP32_BGRX;
	//...
	return SAIL_PIXEL_FORMAT_UNKNOWN;
}

helix::Bitmap::ConvertInfo<helix::PixelFormat, SailPixelFormat> helix::Bitmap::formatConvert(SailPixelFormat format)
{
	switch (format)
	{
		case SAIL_PIXEL_FORMAT_BPP32_RGBA:
			return {helix::PixelFormat::RGBA8UNorm};
		case SAIL_PIXEL_FORMAT_BPP64_RGBA:
			return {helix::PixelFormat::RGBA16UNorm};
		case SAIL_PIXEL_FORMAT_BPP128:
			return {helix::PixelFormat::RGBA32Float};
		case SAIL_PIXEL_FORMAT_BPP96:
			return {helix::PixelFormat::RGB32Float};
		case SAIL_PIXEL_FORMAT_BPP64:
			return {helix::PixelFormat::RG32Float};
		case SAIL_PIXEL_FORMAT_BPP32:
			return {helix::PixelFormat::R32Float};
		case SAIL_PIXEL_FORMAT_BPP32_BGRA:
			return {helix::PixelFormat::BGRA8UNorm};
		case SAIL_PIXEL_FORMAT_BPP24_BGR:
			return {helix::PixelFormat::BGRX8UNorm, true, SAIL_PIXEL_FORMAT_BPP32_BGRX};
		case SAIL_PIXEL_FORMAT_BPP32_BGRX:
			return {helix::PixelFormat::BGRX8UNorm};
		case SAIL_PIXEL_FORMAT_BPP24_RGB:
			return {helix::PixelFormat::RGBA8UNorm, true, SAIL_PIXEL_FORMAT_BPP32_RGBA};
		case SAIL_PIXEL_FORMAT_UNKNOWN: [[fallthrough]];
		default:
			break;
	}
	//...
	return {helix::PixelFormat::Unknown};
}

bool helix::Bitmap::imageConvertFormat(sail::image& image, SailPixelFormat dstFormat)
{
	if (!image.can_convert(dstFormat))
	{
		Logger::info(u8"Bitmap无法转换该PixelFormat");
		return false;
	}
	image.convert(dstFormat);
	return true;
}

const helix::PixelFormat& helix::Texture2D::getPixelFormat() const
{
	return format;
}

helix::Vector2UI32 helix::Texture2D::getSize() const
{
	return size;
}

helix::Texture2D::Type helix::Texture2D::getType() const
{
	return type;
}

void helix::Texture2D::setPixelFormat(const PixelFormat& format)
{
	this->format = format;
}

void helix::Texture2D::setSize(Vector2UI32 size)
{
	this->size = size;
}

void helix::Texture2D::setType(Type type)
{
	this->type = type;
}
