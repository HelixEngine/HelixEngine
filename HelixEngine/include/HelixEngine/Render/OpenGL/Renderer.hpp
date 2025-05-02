#pragma once
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Core/Window.hpp>

namespace helix
{
	class Window;
}

namespace helix::opengl
{

	class Renderer : public helix::Renderer
	{
		friend class Window;
	public: //记得删

		void startRun() override;

		void renderLoopFunc();
		std::stop_token token;

		//cmd process//

		RenderCommand* cmd;
		SDL_GLContext context;
		void initRender();
		void cmdProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;
	};
}
