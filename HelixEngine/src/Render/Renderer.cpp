#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>
#include <HelixEngine/Core/Window.hpp>

#include "HelixEngine/Util/Logger.hpp"

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
	Renderer(nullptr)
{
}

helix::Renderer::Renderer(Window* window) :
	window(window)
{
	if (!window)
	{
		Logger::error(u8"Renderer的构造函数传入了空的Window指针");
		return;
	}
	RenderThreadInstance threadInstance{};
	threadInstance.renderer = this;
	threadInstance.qWidget = std::move(window->getQWidget());
	renderThread = std::jthread{renderInstance->getThreadFunc(), threadInstance};
}

helix::Renderer::~Renderer()
{
	closeRenderThread();
}

const helix::Ref<helix::RenderQueue>& helix::Renderer::getRenderQueue() const
{
	return renderQueue;
}

void helix::Renderer::begin(Color clearColor) const
{
	renderQueue->addCommand<BeginCommand>(RenderCommand{RenderCommand::Type::Begin}, clearColor);
}

void helix::Renderer::end() const
{
	renderQueue->addCommand<RenderCommand>(RenderCommand::Type::End);
	renderQueue->swapFrontBuffer();
}

void helix::Renderer::closeRenderThread()
{
	window->closeUpdateThread();
	//交换一个空的buffer，通知RenderThread退出
	//这个操作可能后续给UpdateThread做
	std::unique_lock lock{renderQueue->mtx};
	renderQueue->tempBuffer->commands.clear();
	renderQueue->isFrontBufferReady = true;
	lock.unlock();
	if (renderThread.joinable())
		renderThread.join();
}
