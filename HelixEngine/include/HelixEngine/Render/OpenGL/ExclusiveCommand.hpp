#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>

namespace helix::opengl
{
	struct CreateGLShaderCommand final : ResourceCommand
	{
		CreateGLShaderCommand() { type = Type::CreateGLShader; }
		Shader* shader{};
		std::u8string shaderCode{};
	};

	struct CreateGLRenderPipelineCommand final : ResourceCommand
	{
		CreateGLRenderPipelineCommand() { type = Type::CreateGLRenderPipeline; }
		RenderPipeline* renderPipeline{};
		RenderPipeline::Config config{};
	};
}
