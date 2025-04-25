#include <vulkan/vulkan.h>
#include <QWindow>
#include <HelixEngine/Render/QRhi/RenderInstance.hpp>
#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <HelixEngine/Render/Command/BeginCommand.hpp>

std::function<void(helix::RenderThreadInstance)> helix::q_rhi::RenderInstance::getThreadFunc()
{
	return threadFunc;
}

std::unique_ptr<QRhi> helix::q_rhi::RenderInstance::createRhi(RenderLoopInstance& renderLoopInstance)
{
	std::unique_ptr<QRhi> rhi;
	auto qWindow = renderLoopInstance.qWidget ? renderLoopInstance.qWidget->windowHandle() : nullptr;
	switch (auto graphicsApi = GraphicsApi::Vulkan)
	{
#if QT_CONFIG(vulkan)
		case GraphicsApi::Vulkan:
		{
			QRhiVulkanInitParams vkInitParams{};
			renderLoopInstance.vulkanInstance = createQVulkanInstance();
			auto vkInst = renderLoopInstance.vulkanInstance.get();
			vkInitParams.inst = vkInst;

			if (qWindow)
			{
				qWindow->setSurfaceType(QSurface::VulkanSurface);
				qWindow->setVulkanInstance(vkInst);
				vkInitParams.window = qWindow;
			}

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
			if (qWindow)
				qWindow->setSurfaceType(QSurface::Direct3DSurface);
			QRhiD3D12InitParams d3d12InitParams;
#ifdef HELIX_DEBUG
			d3d12InitParams.enableDebugLayer = true;
#endif
			rhi.reset(QRhi::create(QRhi::D3D12, &d3d12InitParams));
			break;
		}
		case GraphicsApi::D3D11:
		{
			if (qWindow)
				qWindow->setSurfaceType(QSurface::Direct3DSurface);
			QRhiD3D11InitParams d3d11InitParams;
#ifdef HELIX_DEBUG
			d3d11InitParams.enableDebugLayer = true;
#endif
			rhi.reset(QRhi::create(QRhi::D3D11, &d3d11InitParams));
			break;
		}
#endif
#if QT_CONFIG(opengles2)
		case GraphicsApi::OpenGLES2:
		{
			if (qWindow)
				qWindow->setSurfaceType(QSurface::OpenGLSurface);
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

std::unique_ptr<QVulkanInstance> helix::q_rhi::RenderInstance::createQVulkanInstance()
{
	auto vkInstance = std::make_unique<QVulkanInstance>();
	vkInstance->setExtensions(QRhiVulkanInitParams::preferredInstanceExtensions());
#ifdef HELIX_DEBUG
	vkInstance->setLayers({"VK_LAYER_KHRONOS_validation"});
#endif
	if (!vkInstance->create())
	{
		Logger::error(u8"创建QVulkanInstance失败");
	}
	return vkInstance;
}

void helix::q_rhi::RenderInstance::threadFunc(RenderThreadInstance threadInstance)
{
	RenderLoopInstance renderLoopInstance;
	renderLoopInstance.threadInstance = &threadInstance;
	renderLoopInstance.qWidget = threadInstance.qWidget;
	auto& qWidget = renderLoopInstance.qWidget;
	auto graphicsApi = GraphicsApi::Vulkan;
	if (qWidget)
	{
		auto qWindow = qWidget->windowHandle();

		switch (graphicsApi)
		{
#if QT_CONFIG(vulkan)
			case GraphicsApi::Vulkan:
				qWindow->setSurfaceType(QSurface::VulkanSurface);
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
				qWindow->setSurfaceType(QSurface::Direct3DSurface);
				break;
#endif
#if QT_CONFIG(opengles2)
			case GraphicsApi::OpenGLES2:
				qWin->setSurfaceType(QSurface::OpenGLSurface);
			break;
#endif
			default: ;
		}
	}
	renderLoopInstance.rhi = createRhi(renderLoopInstance);
	auto& rhi = renderLoopInstance.rhi;

	auto& swapChain = renderLoopInstance.swapChain;
	std::unique_ptr<QRhiRenderPassDescriptor> renderPassDescriptor;
	if (qWidget)
	{
		swapChain.reset(rhi->newSwapChain());
		swapChain->setWindow(qWidget->windowHandle());
		renderPassDescriptor.reset(swapChain->newCompatibleRenderPassDescriptor());
		swapChain->setRenderPassDescriptor(renderPassDescriptor.get());
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

	if (qWidget)
	{
		renderLoop(renderLoopInstance);
	} else
	{
		offscreenRenderLoop(renderLoopInstance);
	}
}

void helix::q_rhi::RenderInstance::renderLoop(const RenderLoopInstance& renderLoopInstance)
{
	auto& rhi = renderLoopInstance.rhi;
	auto renderer = renderLoopInstance.threadInstance->renderer;
	auto& swapChain = renderLoopInstance.swapChain;
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

void helix::q_rhi::RenderInstance::offscreenRenderLoop(const RenderLoopInstance& renderLoopInstance)
{
	auto& rhi = renderLoopInstance.rhi;
	auto renderer = renderLoopInstance.threadInstance->renderer;
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
