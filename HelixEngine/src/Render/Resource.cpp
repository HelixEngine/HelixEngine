#include <HelixEngine/Render/Resource.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>
#include <filesystem>

#include "HelixEngine/Util/MemoryBlock.hpp"

helix::Ref<helix::Bitmap> helix::Bitmap::load(const std::u8string& filePath, const Config& config)
{
	Ref bitmap = new Bitmap;
	bitmap->setName(u8"Bitmap:" + filePath);
	bitmap->innerLoad(filePath, config);
	return bitmap;
}

void helix::Bitmap::innerLoad(const std::u8string& filePath, Config config)
{
	int imreadFlag = cv::IMREAD_COLOR_BGR;
	if (config.bitmapFormat == PixelFormat::Unknown)
	{

	}
	cvBitmap = cv::imread(reinterpret_cast<const char*>(filePath.c_str()), cv::IMREAD_UNCHANGED);

	CV_8UC3;
}
