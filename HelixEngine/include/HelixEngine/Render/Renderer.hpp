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
	struct LoadBitmapCommand;
}

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
		[[nodiscard]] Ref<Bitmap> loadBitmap(std::u8string filePath, Bitmap::Config config = Bitmap::Config{}) const;
		[[nodiscard]] Ref<Texture2D> createTexture2D(Ref<Bitmap> bitmap,
		                                             const PixelFormat& textureFormat = PixelFormat::Unknown,
		                                             Texture2D::Type type = Texture2D::Type::Default) const;
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
		[[nodiscard]] virtual Ref<Texture2D> createNativeTexture2D(
				Ref<Bitmap> bitmap,
				const PixelFormat& textureFormat, Texture2D::Type type) const = 0;
		std::jthread renderThread;
		static void innerLoadBitmap(const LoadBitmapCommand* cmd);
	private:
		//Game run

		virtual void startRun() = 0;
		virtual void readyRender() = 0;
		virtual void renderThreadFunc(const std::stop_token& token) = 0;
	};
}
