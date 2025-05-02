#pragma once
#include <functional>
#include <thread>
#include <HelixEngine/Render/CommandQueue.hpp>
#include <HelixEngine/Render/Resource.hpp>
#include <HelixEngine/Util/Color.hpp>
#include <HelixEngine/Util/MemoryBlock.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace helix
{
	using RenderList = CommandList<RenderCommand>;
	using RenderQueue = CommandQueue<RenderCommand>;
	using ResourceList = CommandList<ResourceCommand>;
	using ResourceQueue = CommandQueue<ResourceCommand>;

	class Renderer : public Object
	{
		friend class Window;
		friend class Game;
	public:
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;
		[[nodiscard]] const Ref<ResourceQueue>& getResourceQueue() const;
		[[nodiscard]] Window* getWindow() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;
		[[nodiscard]] virtual Ref<VertexBuffer> createVertexBuffer(
				VertexBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const = 0;
	private:
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Ref<ResourceQueue> resourceQueue = new ResourceQueue;
		std::jthread renderThread;
		std::jthread resourceThread;
		Window* window = nullptr;
	protected:
		using CommandProcessThreadFunc = std::function<void(const std::stop_token&)>;
		void startRenderThread(CommandProcessThreadFunc func);
		void startResourceThread(CommandProcessThreadFunc func);
	private:
		//Game run

		virtual void startRun() = 0;
	};
}
