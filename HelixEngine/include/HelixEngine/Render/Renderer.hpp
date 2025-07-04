#pragma once
#include <functional>
#include <thread>
#include <HelixEngine/Render/SyncCommand.hpp>
#include <HelixEngine/Render/Resource.hpp>
#include <HelixEngine/Util/Color.hpp>
#include <HelixEngine/Util/MemoryBlock.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Render/Command/RenderCommand.hpp>

namespace helix
{
	using RenderList = CommandList<RenderCommand>;
	using RenderQueue = CommandQueue<RenderCommand>;

	class Renderer : public Object
	{
		friend class Window;
		friend class Game;
	public:
		~Renderer() override;
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;
		[[nodiscard]] Window* getWindow() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;

		//Resource Command

		[[nodiscard]] Ref<MemoryBuffer>
		createMemoryBuffer(MemoryBuffer::Type type, MemoryBuffer::Usage usage, Ref<MemoryBlock> bufferData) const;
		void setRenderPipeline(Ref<RenderPipeline> renderPipeline) const;
		void setPrimitiveTopology(PrimitiveTopology primitiveTopology) const;
		void setViewport(Viewport viewport) const;
		void setScissor(Scissor scissor) const;
		void draw(uint32_t vertexCount) const;
		void drawIndexed(uint32_t indexCount) const;
	private:
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Window* window = nullptr;
	protected:
		using CommandProcessThreadFunc = std::function<void(const std::stop_token&)>;
		[[nodiscard]] virtual Ref<MemoryBuffer> createNativeMemoryBuffer(
				MemoryBuffer::Type type,
				MemoryBuffer::Usage usage, Ref<MemoryBlock> vertexData) const = 0;
		std::jthread renderThread;
	private:
		//Game run

		virtual void startRun() = 0;
		virtual void readyRender() = 0;
		virtual void renderThreadFunc(const std::stop_token& token) = 0;
	};
}
