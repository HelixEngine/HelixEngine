#include <HelixEngine/Render/RenderQueue.hpp>

std::unique_ptr<helix::RenderCommand> helix::RenderCommandBuffer::acquireCommand()
{
	auto command = std::move(commands.front());
	commands.pop_front();
	return command;
}

bool helix::RenderCommandBuffer::empty() const
{
	return commands.empty();
}

void helix::RenderQueue::swapFrontBuffer()
{
	std::unique_lock lock{mtx};
	tempBuffer.swap(frontBuffer);
	frontBuffer->commands.clear();
	isFrontBufferReady = true;
}

void helix::RenderQueue::swapBackBuffer()
{
	std::unique_lock lock{mtx};
	if (!isFrontBufferReady)
		return;
	backBuffer.swap(tempBuffer);
	isFrontBufferReady = false;
}

void helix::RenderQueue::setBackBuffer(Ref<RenderCommandBuffer> buffer)
{
	backBuffer = std::move(buffer);
}

const helix::Ref<helix::RenderCommandBuffer>& helix::RenderQueue::getBackBuffer() const
{
	return backBuffer;
}

const helix::Ref<helix::RenderCommandBuffer>& helix::RenderQueue::getFrontBuffer() const
{
	return frontBuffer;
}
