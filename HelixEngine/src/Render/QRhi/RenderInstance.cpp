#include <QWindow>
#include <HelixEngine/Render/QRhi/RenderInstance.hpp>
#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>

void helix::q_rhi::RenderInstance::setGraphicsApi(GraphicsApi api)
{
	if (rhi.isInit())
	{
		Logger::warning(u8"QRhi已经初始化，无法更改图形API");
		return;
	}
	graphicsApi = api;
}

helix::q_rhi::RenderInstance::GraphicsApi helix::q_rhi::RenderInstance::getGraphicsApi()
{
	return graphicsApi;
}

void helix::q_rhi::RenderInstance::threadFunc(RenderThreadInstance threadInstance)
{
	auto& rhi = RenderInstance::rhi.get();

	if (threadInstance.windowId)
	{
		swapChain.reset(rhi->newSwapChain());
		auto qWin = QWindow::fromWinId(threadInstance.windowId);
		switch (graphicsApi)
		{
#if QT_CONFIG(vulkan)
			case GraphicsApi::Vulkan:
				qWin->setSurfaceType(QSurface::VulkanSurface);
				break;
#if QT_CONFIG(metal)
			case GraphicsApi::Metal:
				qWin->setSurfaceType(QSurface::MetalSurface);
			break;
#endif
#endif
#ifdef Q_OS_WIN
			case GraphicsApi::D3D11:
			case GraphicsApi::D3D12:
				qWin->setSurfaceType(QSurface::Direct3DSurface);
				break;
#endif
#if QT_CONFIG(opengles2)
			case GraphicsApi::OpenGLES2:
				qWin->setSurfaceType(QSurface::OpenGLSurface);
			break;
#endif
			default: ;
		}
		swapChain->setWindow(qWin);
		swapChain->createOrResize();
	}

	auto renderer = threadInstance.renderer;
	auto renderQueue = renderer->getRenderQueue();

	// auto bufferCount = (swapChain->flags() & QRhiSwapChain::MinimalBufferCount).toInt();
	// std::vector<Ref<RenderCommandBuffer>> backBuffers(bufferCount);
	// for (auto& buffer: backBuffers)
	// {
	// 	buffer = new RenderCommandBuffer;
	// }

	renderQueue->setBackBuffer(new RenderCommandBuffer);

	if (threadInstance.windowId)
	{
		renderLoop(threadInstance);
	} else
	{
		offscreenRenderLoop(threadInstance);
	}
}

QRhi* helix::q_rhi::RenderInstance::getQRhi()
{
	return rhi.get().get();
}

QRhiSwapChain* helix::q_rhi::RenderInstance::getQRhiSwapChain() const
{
	return swapChain.get();
}

std::unique_ptr<QRhi> helix::q_rhi::RenderInstance::createRhi()
{
	std::unique_ptr<QRhi> rhi;
	switch (graphicsApi)
	{
#if QT_CONFIG(vulkan)
		case GraphicsApi::Vulkan:
		{
			QRhiVulkanInitParams vkInitParams;
			rhi.reset(QRhi::create(QRhi::Vulkan, &vkInitParams));
			break;
		}
#if QT_CONFIG(metal)
		{
			case GraphicsApi::Metal:
				QRhiMetalInitParams mtInitParams;
			rhi.reset(QRhi::create(QRhi::Metal, &mtInitParams));
			break;
		}
#endif
#endif
#ifdef Q_OS_WIN
		case GraphicsApi::D3D12:
		{
			QRhiD3D12InitParams d3D12InitParams;
			rhi.reset(QRhi::create(QRhi::D3D12, &d3D12InitParams));
			break;
		}
		case GraphicsApi::D3D11:
		{
			QRhiD3D11InitParams d3d11InitParams;
			rhi.reset(QRhi::create(QRhi::D3D11, &d3d11InitParams));
			break;
		}
#endif
#if QT_CONFIG(opengles2)
		{
			case GraphicsApi::OpenGLES2:
			QRhiGles2InitParams gles2InitParams;
			rhi.reset(QRhi::create(QRhi::OpenGLES2, &gles2InitParams));
			break;
		}
#endif
		default:
			Logger::error(u8"该图形API当前或当前平台不支持");
	}

	return rhi;
}

void helix::q_rhi::RenderInstance::renderLoop(RenderThreadInstance threadInstance) const
{
	auto& rhi = RenderInstance::rhi.get();
	auto renderer = threadInstance.renderer;
	auto renderQueue = renderer->getRenderQueue();
	while (true)
	{
		auto backBuffer = swapBackBuffer(renderQueue);
		if (backBuffer->empty())
			break;

		std::unique_ptr<RenderCommand> cmd{};
		do
		{
			cmd = backBuffer->acquireCommand();
			auto qCmdBuf = swapChain->currentFrameCommandBuffer();
			auto qRt = swapChain->currentFrameRenderTarget();
			switch (cmd->type)
			{
				case RenderCommand::Type::Begin:
				{
					rhi->beginFrame(swapChain.get());
					auto clearCmd = reinterpret_cast<BeginCommand*>(cmd.get());
					qCmdBuf->beginPass(qRt, static_cast<QColor>(clearCmd->clearColor), {});
					break;
				}
				case RenderCommand::Type::End:
					qCmdBuf->endPass();
					rhi->endFrame(swapChain.get());
					break;
				default:
					Logger::warning(u8"不支持的RenderCommand");
					break;
			}
		} while (cmd->type != RenderCommand::Type::End);
	}
}

void helix::q_rhi::RenderInstance::offscreenRenderLoop(RenderThreadInstance threadInstance)
{
	auto& rhi = RenderInstance::rhi.get();
	auto renderer = threadInstance.renderer;
	auto renderQueue = renderer->getRenderQueue();
	while (true)
	{
		auto backBuffer = swapBackBuffer(renderQueue);
		if (backBuffer->empty())
			break;

		std::unique_ptr<RenderCommand> cmd{};
		do
		{
			cmd = backBuffer->acquireCommand();
			QRhiCommandBuffer* qCmdBuf = nullptr;
			switch (cmd->type)
			{
				case RenderCommand::Type::Begin:
					rhi->beginOffscreenFrame(&qCmdBuf);
					break;
				case RenderCommand::Type::End:
					rhi->endOffscreenFrame();
					break;
				default:
					Logger::warning(u8"不支持的RenderCommand");
					break;
			}
		} while (cmd->type != RenderCommand::Type::End);
	}
}
