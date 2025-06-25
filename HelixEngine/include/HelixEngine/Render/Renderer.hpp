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
	using ResourceList = CommandList<ResourceCommand>;
	using ResourcePipeline = CommandPipeline<ResourceCommand>;

	class Renderer : public Object
	{
		friend class Window;
		friend class Game;
	public:
		~Renderer() override;
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;
		[[nodiscard]] const Ref<ResourcePipeline>& getResourcePipeline() const;
		[[nodiscard]] Window* getWindow() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;

		//Resource Command

		[[nodiscard]] Ref<VertexBuffer>
		createVertexBuffer(VertexBuffer::Usage usage, Ref<MemoryBlock> vertexData) const;
		void setRenderPipeline(Ref<RenderPipeline> renderPipeline) const;
		void setPrimitiveTopology(PrimitiveTopology primitiveTopology) const;
		void draw(uint32_t vertexCount) const;
	private:
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Ref<ResourcePipeline> resourcePipeline = new ResourcePipeline;
		Window* window = nullptr;
		std::jthread renderThread;
	protected:
		using CommandProcessThreadFunc = std::function<void(const std::stop_token&)>;
		[[nodiscard]] virtual Ref<VertexBuffer> createNativeVertexBuffer(
				VertexBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const = 0;
	private:
		void startRenderThread(CommandProcessThreadFunc func);

		//Game run

		virtual void startRun() = 0;
		[[nodiscard]] virtual CommandProcessThreadFunc getRenderThreadFunc() = 0;
	};
}
