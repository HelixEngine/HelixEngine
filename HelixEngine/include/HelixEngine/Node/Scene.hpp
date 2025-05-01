#pragma once
#include <HelixEngine/Node/Node.hpp>

namespace helix
{
	class Window;

	class SceneBase : public Node2D
	{
		Window* window{};
	public:
		[[nodiscard]] Window* getWindow() const;
	protected:
		void render(Renderer* renderer) override = 0;
	};

	class Scene2D final : public SceneBase
	{
		void render(Renderer* renderer) override;
	};
}
