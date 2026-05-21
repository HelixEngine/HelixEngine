#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <filesystem>
#include <HelixEngine/Util/Logger.hpp>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
using namespace helix;

enum class ShaderLanguage
{
	Unknown,
	GLSL,
	HLSL,
	//Slang,
};

enum class ShaderStage
{
	Unknown,
	Vertex,
	Pixel, //Or Fragment
	Compute,
};

ShaderLanguage GetShaderLanguage(std::string_view str)
{
	if (str == "hlsl" || str == "HLSL")
		return ShaderLanguage::HLSL;
	if (str == "glsl" || str == "GLSL")
		return ShaderLanguage::GLSL;
	return ShaderLanguage::Unknown;
}

ShaderStage GetShaderStage(std::string_view str)
{
	if (str == "vertex" || str == "vert")
		return ShaderStage::Vertex;
	if (str == "pixel" || str == "frag" || str == "fragment")
		return ShaderStage::Pixel;
	return ShaderStage::Unknown;
}

EShLanguage GetShaderStageGLSL(ShaderStage stage)
{
	switch (stage)
	{
		case ShaderStage::Unknown:
			break;
		case ShaderStage::Vertex:
			return EShLangVertex;
		case ShaderStage::Pixel:
			return EShLangFragment;
		case ShaderStage::Compute:
			return EShLangCompute;
	}
	return EShLangCount; // as unknown
}

class Includer : public glslang::TShader::Includer
{
public:
	Includer() = default;

	IncludeResult* includeLocal(const char* includeName,const char* includerName,size_t inclusionDepth) override
	{
		std::string content;
		for (const auto& path: includePaths)
		{
			std::ifstream f(path / includeName);
			if (!f.is_open()) continue;
			content = std::string((std::istreambuf_iterator(f)),
							 std::istreambuf_iterator<char>());
		}
		if (content.empty()) return nullptr;

		auto storage = new std::string(std::move(content));
		return new IncludeResult(includeName, storage->data(), storage->size(), storage);
	}
	IncludeResult* includeSystem(const char* n, const char* i, size_t d) override
	{
		return includeLocal(n,i,d);
	}
	void releaseInclude(IncludeResult* r) override
	{
		if (!r) return;
		delete static_cast<std::string*>(r->userData);
		delete r;
	}

	std::vector<std::filesystem::path> includePaths;
};

struct CompileResult
{
	std::u8string glsl;
	std::vector<uint32_t> spirVForOpenGL;
	//std::vector<uint32_t> spirVForVulkan;
};

class Compiler
{
public:
	Compiler(std::string sourcePath, ShaderLanguage language,ShaderStage stage):language(language),stage(stage)
	{
		std::fstream src(std::move(sourcePath));
		if (!src.is_open())
		{
			Logger::error(u8"Source file could not be opened");
			return;
		}
		sourceCode = (std::stringstream{} << src.rdbuf()).str();
	}

	bool compile()
	{
		if (sourceCode.empty())
			return false;

		switch (language)
		{
			case ShaderLanguage::Unknown:
				break;
			case ShaderLanguage::GLSL:
				return compileGLSL();
			case ShaderLanguage::HLSL:
				Logger::error(u8"HLSL is not supported now");
				break;
			default:
				Logger::error(u8"[Logic Error] Invalid shader language into default switch-case");
				break;
		}

		return false;
	}

	std::vector<std::filesystem::path> includePaths;
	CompileResult result;
private:
	std::string sourceCode;
	ShaderLanguage language;
	ShaderStage stage;

	bool compileGLSL()
	{
		result.glsl = reinterpret_cast<const char8_t*>(sourceCode.c_str());
		//SPIR-V For OpenGL
		Includer includer;
		std::vector src = {sourceCode.c_str()};
		glslang::InitializeProcess();
		auto glslStage = GetShaderStageGLSL(stage);
		glslang::TShader shader{glslStage};
		shader.setStrings(src.data(), src.size());
		shader.setEnvInput(glslang::EShSourceGlsl, glslStage, glslang::EShClientOpenGL, 460);
		shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450); //后续vk和gl都要生成spv
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_6);
		//shader.setEntryPoint("false_main"); //考虑no-link下这行需不需要特殊处理
		shader.setSourceEntryPoint("");


		includer.includePaths = includePaths;
		if (!shader.parse(GetDefaultResources(), 460, false, EShMsgDefault, includer))
		{
			Logger::error(std::u8string_view(reinterpret_cast<const char8_t*>(shader.getInfoLog())));
			return false;
		}

		glslang::GlslangToSpv(*shader.getIntermediate(),result.spirVForOpenGL);
		glslang::FinalizeProcess();

		return true;
	}
};

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
		{{"-o","-output"}, 1},
		{{"-l","-language"}, 1},
		{{"-s","-source"}, 1},
		{{"-stage"}, 1},
	};

	if (!clp.process(args))
	{
		Logger::error(u8"Command line parse error");
		return 1;
	}

	if (args[0].argContent.empty())
	{
		Logger::error(u8"No Output File Path Input");
		return 1;
	}
	const auto& outPath = args[0].argContent[0];

	if (args[1].argContent.empty())
	{
		Logger::error(u8"No Source Language Input");
		return 1;
	}
	const auto& langStr = args[1].argContent[0];

	if (args[2].argContent.empty())
	{
		Logger::error(u8"No Source Code File Path Input");
		return 1;
	}
	const auto& srcPath = args[2].argContent[0];

	if (args[3].argContent.empty())
	{
		Logger::error(u8"No Source Stage Input");
		return 1;
	}
	const auto& stageStr = args[3].argContent[0];

	auto lang = GetShaderLanguage(langStr);
	if (lang == ShaderLanguage::Unknown)
	{
		Logger::error(u8"Unknown Source Shader Language");
		return 1;
	}

	auto stage = GetShaderStage(stageStr);
	if (stage == ShaderStage::Unknown)
	{
		Logger::error(u8"Unknown Source Shader Stage");
		return 1;
	}

	Compiler compiler{srcPath,lang,stage};
	compiler.includePaths = {std::filesystem::path(srcPath).parent_path()};
	compiler.compile();

	std::fstream outFile{outPath,std::ios::out | std::ios::binary | std::ios::ate};
	if (!outFile.is_open())
	{
		Logger::error(u8"Failed to open output file");
		return 1;
	}
	//测试SPIRV生成
	outFile.write(reinterpret_cast<const char*>(compiler.result.spirVForOpenGL.data()), static_cast<std::streamsize>(compiler.result.spirVForOpenGL.size() * sizeof(uint32_t) / sizeof(char)));
	outFile.close();

	return 0;
}