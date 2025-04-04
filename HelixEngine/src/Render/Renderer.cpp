#include <HelixEngine/Render/Renderer.hpp>

#include "HelixEngine/Render/Command/BeginCommand.hpp"

bool helix::RenderInstance::queryFeature(RenderFeature feature)
{
	return Renderer::feature.getItem(feature);
}

const helix::Ref<helix::RenderCommandBuffer>& helix::RenderInstance::swapBackBuffer(const Ref<RenderQueue>& renderQueue)
{
	renderQueue->swapBackBuffer();
	return renderQueue->getBackBuffer();
}

void helix::Renderer::setRenderFeature(BitOption<RenderFeature> feature)
{
	Renderer::feature = feature;
}

helix::BitOption<helix::RenderFeature> helix::Renderer::getRenderFeature()
{
	return feature;
}

void helix::Renderer::setRenderInstance(Ref<RenderInstance> instance)
{
	renderInstance = std::move(instance);
}

helix::Renderer::Renderer() :
	Renderer(reinterpret_cast<WId>(nullptr))
{
}

helix::Renderer::Renderer(WId windowId)
{
	RenderThreadInstance threadInstance{};
	threadInstance.renderer = this;
	threadInstance.windowId = windowId;
	renderThread = std::jthread{[this, threadInstance]
	{
		renderInstance->threadFunc(threadInstance);
	}};
}

helix::Renderer::~Renderer()
{
	//交换一个空的buffer，通知RenderThread退出
	//这个操作可能后续给UpdateThread做
	renderQueue->swapFrontBuffer();
}

const helix::Ref<helix::RenderQueue>& helix::Renderer::getRenderQueue() const
{
	return renderQueue;
}

void helix::Renderer::begin(Color clearColor) const
{
	renderQueue->addCommand<BeginCommand>(RenderCommand{RenderCommand::Type::Begin}, std::move(clearColor));
}

void helix::Renderer::end() const
{
	renderQueue->addCommand<RenderCommand>(RenderCommand::Type::End);
	renderQueue->swapFrontBuffer();
}
