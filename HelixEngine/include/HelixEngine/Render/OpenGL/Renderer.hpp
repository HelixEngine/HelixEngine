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

		//cmd dispatch//

		[[nodiscard]] Ref<VertexBuffer> createVertexBuffer(
				VertexBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const override;

		//render thread//

		void startRun() override;

		void renderLoopFunc();
		std::stop_token token;

		//cmd process//

		RenderCommand* cmd{};
		SDL_GLContext context{};
		void initRender();
		void cmdProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;
		void createVertexBufferProc() const;
	};
}
