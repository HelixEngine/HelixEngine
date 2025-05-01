#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Util/Logger.hpp>

namespace helix
{
	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return queue;
	}

	Window* Renderer::getWindow() const
	{
		return window;
	}

	void Renderer::begin(Color clearColor) const
	{
		queue->addCommand<BeginCommand>(RenderCommand{RenderCommand::Type::Begin}, clearColor);
	}

	void Renderer::end() const
	{
		queue->addCommand<RenderCommand>(RenderCommand::Type::End);
		queue->commit();
	}

	void Renderer::startRenderThread(RenderThreadFunc func)
	{
		renderThread = std::jthread{std::move(func)};
	}
}
