#pragma once
#include <HelixEngine/Render/Renderer.hpp>
#include <rhi/qrhi.h>
#include <HelixEngine/Util/DelayInit.hpp>

namespace helix::q_rhi
{
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

		void setGraphicsApi(GraphicsApi api);
		[[nodiscard]] GraphicsApi getGraphicsApi();
		void threadFunc(RenderThreadInstance threadInstance) override;
		[[nodiscard]] QRhi* getQRhi();
		[[nodiscard]] QRhiSwapChain* getQRhiSwapChain() const;
	private:
		std::unique_ptr<QRhi> createRhi();
		GraphicsApi graphicsApi = GraphicsApi::D3D12;
		DelayInit<std::unique_ptr<QRhi>> rhi{[this] { return createRhi(); }};

		void renderLoop(RenderThreadInstance threadInstance) const;
		void offscreenRenderLoop(RenderThreadInstance threadInstance);
		std::unique_ptr<QRhiSwapChain> swapChain;
	};
}
