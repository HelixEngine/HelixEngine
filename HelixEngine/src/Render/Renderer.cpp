#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>

namespace helix
{
	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return renderQueue;
	}

	const Ref<ResourceQueue>& Renderer::getResourceQueue() const
	{
		return resourceQueue;
	}

	Window* Renderer::getWindow() const
	{
		return window;
	}

	void Renderer::begin(Color clearColor) const
	{
		renderQueue->addCommand<BeginCommand>(RenderCommand{RenderCommand::Type::Begin}, clearColor);
	}

	void Renderer::end() const
	{
		renderQueue->addCommand<RenderCommand>(RenderCommand::Type::End);
		renderQueue->commit();
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
