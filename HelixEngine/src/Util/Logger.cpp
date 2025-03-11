#include <string>
#include <fast_io.h>
#include <HelixEngine/Util/Logger.hpp>
#include <format>

helix::Logger::Output helix::Logger::defaultOutput = [](MessageLevel level, std::u8string_view content)
{
	println(fast_io::u8out(),
	        fast_io::u8concat_std(u8"HelixEngine:[", getMessageLevelName(level), u8"] ", content));
	fflush(stdout);
};

void helix::Logger::setDefaultOutput(Output output)
{
	defaultOutput = std::move(output);
}

helix::Logger::Output helix::Logger::getDefaultOutput()
{
	return defaultOutput;
}

std::u8string helix::Logger::getMessageLevelName(const MessageLevel level)
{
	std::u8string levelName;
	switch (level)
	{
		case MessageLevel::Info:
			levelName = u8"Info";
			break;
		case MessageLevel::Warning:
			levelName = u8"Warning";
			break;
		case MessageLevel::Error:
			levelName = u8"Error";
			break;
		default:
			break;
	}
	return levelName;
}
