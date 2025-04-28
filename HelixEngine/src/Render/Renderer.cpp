#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Core/Window.hpp>

#include "HelixEngine/Util/Logger.hpp"

namespace helix
{
	Renderer::~Renderer()
	{
		queue->quit();
	}

	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return queue;
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
}
