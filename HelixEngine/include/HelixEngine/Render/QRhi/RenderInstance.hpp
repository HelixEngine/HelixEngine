#pragma once
#include <HelixEngine/Render/Renderer.hpp>
#include <rhi/qrhi.h>
#include <HelixEngine/Util/DelayInit.hpp>

namespace helix::q_rhi
{
	struct RenderLoopInstance
	{
		RenderThreadInstance* threadInstance;
		std::unique_ptr<QRhi> rhi;
		std::unique_ptr<QRhiSwapChain> swapChain;
		std::shared_ptr<QWidget> qWidget;
		std::unique_ptr<QVulkanInstance> vulkanInstance;
	};

	class RenderInstance final : public helix::RenderInstance
	{
	public:
		enum class GraphicsApi
		{
			Vulkan,
			Metal,
			D3D12,
			D3D11,
			OpenGLES2,
		};

		std::function<void(RenderThreadInstance)> getThreadFunc() override;
	private:
		static std::unique_ptr<QRhi> createRhi(RenderLoopInstance& renderLoopInstance);
		static std::unique_ptr<QVulkanInstance> createQVulkanInstance();

		static void threadFunc(RenderThreadInstance threadInstance);
		static void renderLoop(const RenderLoopInstance& renderLoopInstance);
		static void offscreenRenderLoop(const RenderLoopInstance& renderLoopInstance);
	};
}
