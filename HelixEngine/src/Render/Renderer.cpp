#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/GeneralCommand.hpp>
#include <HelixEngine/Core/Game.hpp>

namespace helix
{
	Renderer::~Renderer()
	{
		Logger::info(u8"destroy renderer");
	}

	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return renderQueue;
	}

	Window* Renderer::getWindow() const
	{
		return window;
	}

	void Renderer::begin(Color clearColor) const
	{
		BeginCommand cmd;
		cmd.type = RenderCommand::Type::Begin;
		cmd.clearColor = clearColor;
		renderQueue->addCommand<BeginCommand>(std::move(cmd));
	}

	void Renderer::end() const
	{
		RenderCommand cmd;
		cmd.type = RenderCommand::Type::End;
		renderQueue->addCommand<RenderCommand>(std::move(cmd));
		renderQueue->commit();
	}

	Ref<MemoryBuffer> Renderer::createMemoryBuffer(MemoryBuffer::Type type, MemoryBuffer::Usage usage,
	                                               Ref<MemoryBlock> bufferData) const
	{
		auto buf = createNativeMemoryBuffer(type, usage, bufferData);
		CreateMemoryBufferCommand cmd;
		cmd.type = RenderCommand::Type::CreateMemoryBuffer;
		cmd.memoryBuffer = buf;
		cmd.bufferData = std::move(bufferData);
		renderQueue->addCommand<CreateMemoryBufferCommand>(std::move(cmd));
		return buf;
	}

	Ref<Bitmap> Renderer::loadBitmap(std::u8string filePath, Bitmap::Config config) const
	{
		Ref bitmap = new Bitmap;
		LoadBitmapCommand cmd;
		cmd.type = RenderCommand::Type::LoadBitmap;
		cmd.filePath = std::move(filePath);
		cmd.config = std::move(config);
		cmd.bitmap = bitmap;
		renderQueue->addCommand<LoadBitmapCommand>(std::move(cmd));
		return bitmap;
	}

	Ref<Texture2D> Renderer::createTexture2D(Ref<Bitmap> bitmap, const PixelFormat& textureFormat,
	                                         Texture2D::Type type) const
	{
		auto tex = createNativeTexture2D(bitmap, textureFormat, type);
		CreateTexture2DFromBitmapCommand cmd;
		cmd.type = RenderCommand::Type::CreateTexture2DFromBitmap;
		cmd.texture2d = tex;
		cmd.textureFormat = textureFormat;
		cmd.bitmap = std::move(bitmap);
		cmd.textureType = type;
		renderQueue->addCommand<CreateTexture2DFromBitmapCommand>(std::move(cmd));
		return tex;
	}

	void Renderer::setRenderPipeline(Ref<RenderPipeline> renderPipeline) const
	{
		SetRenderPipelineCommand cmd;
		cmd.type = RenderCommand::Type::SetRenderPipeline;
		cmd.renderPipeline = std::move(renderPipeline);
		renderQueue->addCommand<SetRenderPipelineCommand>(std::move(cmd));
	}

	void Renderer::setPrimitiveTopology(PrimitiveTopology primitiveTopology) const
	{
		SetPrimitiveTopologyCommand cmd;
		cmd.type = RenderCommand::Type::SetPrimitiveTopology;
		cmd.primitiveTopology = primitiveTopology;
		renderQueue->addCommand<SetPrimitiveTopologyCommand>(std::move(cmd));
	}

	void Renderer::setViewport(Viewport viewport) const
	{
		SetViewportCommand cmd;
		cmd.type = RenderCommand::Type::SetViewport;
		cmd.viewports = {viewport};
		renderQueue->addCommand<SetViewportCommand>(std::move(cmd));
	}

	void Renderer::setScissor(Scissor scissor) const
	{
		SetScissorCommand cmd;
		cmd.type = RenderCommand::Type::SetScissor;
		cmd.scissors = {scissor};
		renderQueue->addCommand<SetScissorCommand>(std::move(cmd));
	}

	void Renderer::draw(uint32_t vertexCount) const
	{
		DrawCommand cmd;
		cmd.type = RenderCommand::Type::Draw;
		cmd.vertexCount = vertexCount;
		renderQueue->addCommand<DrawCommand>(std::move(cmd));
	}

	void Renderer::drawIndexed(uint32_t indexCount) const
	{
		DrawIndexedCommand cmd;
		cmd.type = RenderCommand::Type::DrawIndexed;
		cmd.indexCount = indexCount;
		renderQueue->addCommand<DrawIndexedCommand>(std::move(cmd));
	}

	void Renderer::innerLoadBitmap(const LoadBitmapCommand* cmd)
	{
		cmd->bitmap->innerLoad(cmd->filePath, cmd->config);
		cmd->bitmap->notify();
	}
}
