#pragma once
#include <string>
#include <fast_io.h>
#include <functional>
#include <HelixEngine/Enum.hpp>

namespace helix
{
	class Logger final
	{
	public:
		using Output = std::function<void(MessageLevel level, std::u8string_view content)>;
	private:
		static Output defaultOutput;
		//后续要把Output和Formatter拆开来，现在全在output里面
	public:
		template<typename... Args>
		static void print(MessageLevel level, Args&&... args)
		{
			print(defaultOutput, level, args...);
		}

		template<typename... Args>
		static void info(const std::u8string_view content, Args&&... args)
		{
			print(MessageLevel::Info, content, args...);
		}

		template<typename... Args>
		static void warning(const std::u8string_view content, Args&&... args)
		{
			print(MessageLevel::Warning, content, args...);
		}

		template<typename... Args>
		static void error(const std::u8string_view content, Args&&... args)
		{
			print(MessageLevel::Error, content, args...);
		}

		template<typename... Args>
		static void print(const Output& output, const MessageLevel level, Args&&... args)
		{
			output(level, fast_io::u8concat_std(args...));
		}

		template<typename... Args>
		static void info(const Output& output, const std::u8string_view content, Args&&... args)
		{
			print(output, MessageLevel::Info, content, args...);
		}

		template<typename... Args>
		static void warning(const Output& output, const std::u8string_view content, Args&&... args)
		{
			print(output, MessageLevel::Warning, content, args...);
		}

		template<typename... Args>
		static void error(const Output& output, const std::u8string_view content, Args&&... args)
		{
			print(output, MessageLevel::Error, content, args...);
		}

		static void setDefaultOutput(Output output);
		[[nodiscard]] static Output getDefaultOutput();

		[[nodiscard]] static std::u8string getMessageLevelName(MessageLevel level);
	};

	// class Formatter final
	// {
	// 	std::string formatTemplate;
	// public:
	// 	//using FormatCallback = std::function<std::string(std::string)>;
	// 	Formatter() = default;
	// 	explicit Formatter(std::string formatTemplate);
	// 	void setFormatTemplate(std::string formatTemplate);
	// 	[[nodiscard]] std::string getFormatTemplate() const;
	//
	// 	template<typename... Args>
	// 	std::string operator()(Args&&... args);
	// };
	//
	// template<typename... Args>
	// std::string Formatter::operator()(Args&&... args)
	// {
	// 	return std::format(formatTemplate, args...);
	// }
}
