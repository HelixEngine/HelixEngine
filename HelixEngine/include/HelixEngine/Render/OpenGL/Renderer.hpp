#pragma once
#include <HelixEngine/Render/Renderer.hpp>

#include "HelixEngine/Core/Window.hpp"

namespace helix_sdl3
{
	class Window;
}

namespace helix_render2::opengl
{

	class Renderer : public helix_render2::Renderer
	{
		friend class Window;
	public: //记得删
		explicit Renderer(helix_sdl3::Window* window);

		struct LoopData
		{
			helix_sdl3::Window* window{};
			Ref<RenderQueue> renderQueue;
		};

		static void renderLoopFunc(const LoopData& loopData);
	};
}
