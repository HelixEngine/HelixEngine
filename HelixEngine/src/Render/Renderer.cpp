#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/Command/GeneralCommand.hpp>

namespace helix
{
	Renderer::~Renderer()
	{
	}

	const Ref<RenderQueue>& Renderer::getRenderQueue() const
	{
		return renderQueue;
	}

	const Ref<ResourcePipeline>& Renderer::getResourcePipeline() const
	{
		return resourcePipeline;
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

	Ref<VertexBuffer> Renderer::createVertexBuffer(VertexBuffer::Usage usage, Ref<MemoryBlock> vertexData) const
	{
		auto vb = createNativeVertexBuffer(usage, vertexData);
		CreateVertexBufferCommand cmd;
		cmd.type = SharedResourceCommand::Type::CreateVertexBuffer;
		cmd.vertexBuffer = vb;
		cmd.vertexData = std::move(vertexData);
		getSharedResourcePipeline()->addCommand<CreateVertexBufferCommand>(std::move(cmd));
		return vb;
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

	void Renderer::startMainRenderThread(std::jthread& mainRenderThread, const bool& isRunning)
	{
		mainRenderThread = std::jthread([&]()
		{
			while (isRunning)
			{
				//渲染调度

				//2.sharedResourceProc(opengl)
				{
					Window* sharedResourceProcWindow = nullptr;
					for (auto window: Window::getAllWindows())
					{
						if (window->getGraphicsApi() == GraphicsApi::OpenGL)
						{
							sharedResourceProcWindow = window;
							break;
						}
					}

					if (sharedResourceProcWindow)
						std::jthread resourceThread([sharedResourceProcWindow]
						{
							sharedResourceProcWindow->getRenderer()->sharedResourceWorkload();
						});
				}

				//3.renderProc
				{
					std::vector<std::jthread> renderThreads(Window::getAllWindows().size());
					for (size_t i = 0; i < renderThreads.size(); ++i)
					{
						renderThreads[i] = std::jthread([i]
						{
							Window::getAllWindows()[i]->getRenderer()->renderWorkload();
						});
					}

					for (auto& renderThread: renderThreads)
					{
						renderThread.join();
					}
				}
			}
		});
	}
}
