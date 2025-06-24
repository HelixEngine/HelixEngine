#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/GeneralCommand.hpp>

namespace helix
{
	Renderer::~Renderer()
	{
	}

	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return renderQueue;
	}

	const Ref<ResourcePipeline>& Renderer::getResourcePipeline() const
	{
		return resourcePipeline;
	}

	Window* Renderer::getWindow() const
	{
		return window;
	}

	void Renderer::begin(Color clearColor) const
	{
		BeginCommand cmd;
		cmd.type = RenderCommand::Type::Begin;
		cmd.clearColor = clearColor;
		renderQueue->addCommand<BeginCommand>(std::move(cmd));
	}

	void Renderer::end() const
	{
		RenderCommand cmd;
		cmd.type = RenderCommand::Type::End;
		renderQueue->addCommand<RenderCommand>(std::move(cmd));
		renderQueue->commit();
	}

	Ref<VertexBuffer> Renderer::createVertexBuffer(VertexBuffer::Usage usage, Ref<MemoryBlock> vertexData) const
	{
		auto vb = createNativeVertexBuffer(usage, vertexData);
		CreateVertexBufferCommand cmd;
		cmd.type = ResourceCommand::Type::CreateVertexBuffer;
		cmd.vertexBuffer = vb;
		cmd.vertexData = std::move(vertexData);
		resourcePipeline->addCommand<CreateVertexBufferCommand>(std::move(cmd));
		return vb;
	}

	void Renderer::setRenderPipeline(Ref<RenderPipeline> renderPipeline) const
	{
		SetRenderPipelineCommand cmd;
		cmd.type = RenderCommand::Type::SetRenderPipeline;
		cmd.renderPipeline = std::move(renderPipeline);
		renderQueue->addCommand<SetRenderPipelineCommand>(std::move(cmd));
	}

	void Renderer::setPrimitiveTopology(PrimitiveTopology primitiveTopology) const
	{

	}

	void Renderer::startRenderThread(CommandProcessThreadFunc func)
	{
		renderThread = std::jthread{std::move(func)};
	}
}
