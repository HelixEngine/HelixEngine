#pragma once
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Core/Window.hpp>

namespace helix
{
	class Window;
}

namespace helix::opengl
{
	class Renderer final : public helix::Renderer
	{
		friend class Window;

		//resource method//

		[[nodiscard]] Ref<VertexBuffer> createNativeVertexBuffer(
				VertexBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const override;

		//render thread//

		void startRun() override;

		void renderLoopFunc();
		std::stop_token renderToken;
		void resourceLoopFunc();
		std::stop_token resourceToken;

		//cmd process//

		RenderCommand* renderCmd{};
		ResourceCommand* resourceCmd{};
		SDL_GLContext renderContext{};
		SDL_GLContext resourceContext{};
		void initGlad(SDL_GLContext* outContext) const;

		void renderProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;

		void resourceProc(const ResourcePipeline::ListRef& list);
		void createVertexBufferProc() const;
	};
}
