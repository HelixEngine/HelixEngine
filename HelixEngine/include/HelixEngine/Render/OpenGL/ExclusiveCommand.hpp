#pragma once
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <glad/glad.h>

#include "Resource.hpp"

namespace helix::opengl
{
	struct CreateGLShaderCommand final : RenderCommand
	{
		Ref<Shader> shader{};
		std::u8string shaderCode{};
	};

	struct CreateGLRenderPipelineCommand final : RenderCommand
	{
		Ref<RenderPipeline> renderPipeline{};
		RenderPipeline::Config config{};
	};

	struct CreateGLVertexArrayCommand final : RenderCommand
	{
		Ref<VertexArray> vertexArray{};
		VertexArray::Config config{};
	};

	struct SetGLVertexArrayCommand final : RenderCommand
	{
		Ref<VertexArray> vertexArray{};
	};

	struct SetGLUniformBindingAttributeCommand final : RenderCommand
	{
		std::vector<UniformBindingAttribute> uniformBindingAttributes{};
	};

	struct DestroyGLShaderCommand final : RenderCommand
	{
		GLuint shaderGL{};
	};

	struct SetGLTexture2DUnitCommand final : RenderCommand
	{
		Texture2DUnit unit{};
	};
}
