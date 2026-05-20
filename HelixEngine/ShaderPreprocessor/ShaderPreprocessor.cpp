#include <iostream>
#include <variant>
#include <HelixEngine/Util/Logger.hpp>
#include <glslang/Public/ShaderLang.h>
using namespace helix;

class CommandLineProcessor
{
public:

	struct Arg
	{
		std::vector<std::string> argNames;
		size_t argSize = 0;
		std::vector<std::string> argContent;
	};

	CommandLineProcessor(int argc,char** argv) : argc(argc), argv(argv)
	{

	}

	bool process(std::vector<Arg>& args)
	{
		if (argc < 2)
		{
			Logger::info(u8"No Input");
			return false;
		}

		if (args.empty())
		{
			Logger::info(u8"No Arg Parsed Rule");
			return false;
		}

		//参数解析
		for (int i = 1; i < argc;)
		{
			bool isParsed = false;
			for (auto& arg : args)
			{
				bool found = false;
				for (const auto & arg_name: arg.argNames)
				{
					if (arg_name == argv[i])
					{
						found = true;
						break;
					}
				}
				if (!found) continue;
				++i;
				if (argc - i < arg.argSize)
				{
					Logger::error(u8"arguments input error");
					return false;
				}
				arg.argContent.resize(arg.argSize);
				for (size_t j = 0; j < arg.argSize; ++j)
				{
					arg.argContent[j] = argv[i];
					++i;
				}
				isParsed = true;
				break;
			}
			if (!isParsed)
			{
				Logger::error(u8"Unparsed argument: ", std::u8string(reinterpret_cast<char8_t*>(argv[i])));
				return false;
			}
		}
		return true;
	}
	int argc;
	char** argv;
};

int main(int argc, char* argv[])
{
	Logger::setDefaultOutput([](MessageLevel level, std::u8string_view content)
	{
		println(fast_io::u8out(),
				fast_io::u8concat_std(u8"Shader Preprocessor:[", Logger::getMessageLevelName(level), u8"] ", content));
		fflush(stdout);
	});

	CommandLineProcessor clp(argc,argv);
	std::vector<CommandLineProcessor::Arg> args = {
		{{"-o"}, 1},
		{{"-o"}, 1},
	};

	if (!clp.process(args))
	{
		Logger::error(u8"Command line parse error");
		return 1;
	}

	return 0;
}