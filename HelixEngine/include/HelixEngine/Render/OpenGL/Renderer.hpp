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

		std::jthread renderThread;
		static inline std::jthread resourceThread;
		static inline bool isResourceThreadRunning = false;
		static inline bool isGladInitialized = false;

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
		[[nodiscard]] SDL_GLContext createSDLContext() const;
		void makeCurrentContext(SDL_GLContext context) const;
		[[nodiscard]] SDL_GLContext createCurrentSDLContext() const;

		void renderProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;

		void resourceProc(const ResourcePipeline::ListRef& list);
		void createVertexBufferProc() const;
	};
}
