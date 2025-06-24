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

	struct CreateGLVertexArrayCommand final : ResourceCommand
	{
		VertexArray* vertexArray{};
		VertexArray::Config config{};
	};

	struct SetGLVertexArrayCommand final : RenderCommand
	{
		Ref<VertexArray> vertexArray{};
	};

	struct DestroyGLShaderCommand final : ResourceCommand
	{
		GLuint shaderGL{};
	};
}
