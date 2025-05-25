#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <glad/glad.h>

namespace helix::opengl
{
	struct CreateGLShaderCommand final : ResourceCommand
	{
		Shader* shader{};
		std::u8string shaderCode{};
	};

	struct CreateGLRenderPipelineCommand final : ResourceCommand
	{
		RenderPipeline* renderPipeline{};
		RenderPipeline::Config config{};
	};

	struct DestroyGLShaderCommand final : ResourceCommand
	{
		GLuint shaderGL{};
	};
}
