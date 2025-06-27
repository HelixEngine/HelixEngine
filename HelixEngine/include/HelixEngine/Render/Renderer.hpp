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
	using SharedResourceList = CommandList<SharedResourceCommand>;
	using SharedResourcePipeline = CommandPipeline<SharedResourceCommand>;

	class Renderer : public Object
	{
		friend class Window;
		friend class Game;
	public:
		~Renderer() override;
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;
		[[nodiscard]] const Ref<ResourcePipeline>& getResourcePipeline() const;
		[[nodiscard]] virtual const Ref<SharedResourcePipeline>& getSharedResourcePipeline() const = 0;
		[[nodiscard]] Window* getWindow() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;

		//Resource Command

		[[nodiscard]] Ref<MemoryBuffer>
		createMemoryBuffer(MemoryBuffer::Usage usage, Ref<MemoryBlock> bufferData) const;
		void setRenderPipeline(Ref<RenderPipeline> renderPipeline) const;
		void setPrimitiveTopology(PrimitiveTopology primitiveTopology) const;
		void setViewport(Viewport viewport) const;
		void setScissor(Scissor scissor) const;
		void draw(uint32_t vertexCount) const;
		void drawIndexed(uint32_t indexCount) const;
	private:
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Ref<ResourcePipeline> resourcePipeline = new ResourcePipeline;
		Window* window = nullptr;
	protected:
		using CommandProcessThreadFunc = std::function<void(const std::stop_token&)>;
		[[nodiscard]] virtual Ref<MemoryBuffer> createNativeMemoryBuffer(
				MemoryBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const = 0;
	private:
		//Game run

		virtual void startRun() = 0;

		virtual void readyRender() = 0;

		virtual void sharedResourceWorkload() = 0;
		virtual void renderWorkload() = 0;

		static void startMainRenderThread(std::jthread& mainRenderThread, const bool& isRunning);
	};
}
