#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>

#include "HelixEngine/Render/Command/CreateVertexBufferCommand.hpp"

namespace helix
{
	Renderer::~Renderer()
	{
		resourcePipeline->quit();
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
		cmd.clearColor = clearColor;
		renderQueue->addCommand<BeginCommand>(cmd);
	}

	void Renderer::end() const
	{
		renderQueue->addCommand<RenderCommand>(RenderCommand::Type::End);
		renderQueue->commit();
	}

	Ref<VertexBuffer> Renderer::createVertexBuffer(VertexBuffer::Usage usage, Ref<MemoryBlock> vertexData) const
	{
		auto vb = createNativeVertexBuffer(usage, vertexData);
		CreateVertexBufferCommand cmd;
		cmd.vertexBuffer = vb;
		cmd.vertexData = std::move(vertexData);
		resourcePipeline->addCommand<CreateVertexBufferCommand>(cmd);
		return vb;
	}

	void Renderer::startRenderThread(CommandProcessThreadFunc func)
	{
		renderThread = std::jthread{std::move(func)};
	}

	void Renderer::startResourceThread(CommandProcessThreadFunc func)
	{
		resourceThread = std::jthread{std::move(func)};
	}
}
