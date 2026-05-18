#include <variant>
#include <HelixEngine/Util/Logger.hpp>
using namespace helix;

class CommandLineProcessor
{
public:

	struct Arg
	{
		std::vector<std::string> argNames;
		size_t argSize = 0;
		std::variant<std::string,std::vector<std::string>> argContent;
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
			for (auto& arg : args)
			{
				++i;
				//上次的进度
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
	};

	clp.process(args);

	return 0;
}