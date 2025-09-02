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
		[[nodiscard]] Ref<Texture2D> createTexture2D(Ref<Bitmap> bitmap,
		                                             const PixelFormat& textureFormat = PixelFormat::Unknown,
		                                             Texture2D::Type type = Texture2D::Type::Default) const;
		[[nodiscard]] Ref<Texture2D> createTexture2D(Texture2D::BitmapConfig config) const;

		[[nodiscard]] Ref<Sampler> createSampler(const Sampler::Config& config = {}) const;
		void setRenderPipeline(Ref<RenderPipeline> renderPipeline) const;
		void setPrimitiveTopology(PrimitiveTopology primitiveTopology) const;
		void setViewport(Viewport viewport) const;
		void setScissor(Scissor scissor) const;
		void draw(uint32_t vertexCount) const;
		void drawIndexed(uint32_t indexCount) const;
		Ref<RenderPipeline> createRenderPipeline(auto&& vertex, auto&& pixel,
		                                         std::source_location location = std::source_location::current());
		[[nodiscard]] Ref<Image> loadImage(std::u8string_view filePath) const;
		[[nodiscard]] GraphicsApi getGraphicsApi() const;
	private:
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Window* window = nullptr;
		GraphicsApi graphicsApi{};
		static inline std::unordered_map<GraphicsApi, std::unique_ptr<Image::Cache>> imageCaches;
	protected:
		[[nodiscard]] virtual Ref<MemoryBuffer> createNativeMemoryBuffer(
				MemoryBuffer::Type type,
				MemoryBuffer::Usage usage, Ref<MemoryBlock> vertexData) const = 0;
		[[nodiscard]] virtual Ref<Texture2D> createNativeTexture2D(Texture2D::BitmapConfig config) const = 0;
		[[nodiscard]] virtual Ref<Sampler> createNativeSampler(const Sampler::Config& config) const = 0;
		[[nodiscard]] virtual Ref<RenderPipeline> createNativeRenderPipeline(RenderPipeline::Config config) const = 0;
		[[nodiscard]] virtual EmbeddedShader::ShaderCodeCompiler::CompilerOption getCompilerOption() const = 0;

		//cmd native
		[[nodiscard]] virtual Ref<Shader> createNativeShader(
				Shader::Usage usage,
				const EmbeddedShader::ShaderCodeCompiler& compiler) = 0;

		std::jthread renderThread;
	private:
		//Game run

		virtual void startRun() = 0;
		virtual void readyRender() = 0;
		virtual void renderThreadFunc(const std::stop_token& token) = 0;
	};

	Ref<RenderPipeline> Renderer::createRenderPipeline(auto&& vertex, auto&& pixel, std::source_location location)
	{
		EmbeddedShader::RasterizedPipelineObject object = EmbeddedShader::RasterizedPipelineObject::compile(
				std::forward<decltype(vertex)>(vertex),
				std::forward<decltype(pixel)>(pixel), getCompilerOption(), location);
		RenderPipeline::Config config;
		config.vertex = createNativeShader(Shader::Usage::Vertex, *object.vertex);
		config.pixel = createNativeShader(Shader::Usage::Pixel, *object.fragment);
		return createNativeRenderPipeline(std::move(config));
	}
}
