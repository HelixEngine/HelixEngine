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
		explicit Renderer(Window* window);

		struct LoopData
		{
			Window* window{};
			Ref<RenderQueue> renderQueue;
		};

		static void renderLoopFunc(const LoopData& loopData);
	};
}
