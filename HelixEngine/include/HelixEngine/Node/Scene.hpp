#pragma once
#include <HelixEngine/Node/Node.hpp>

namespace helix
{
	class Window;

	class Scene2D final : public Node2D
	{
		friend class Window;
		Window* window{};
		void setWindow(Window* window) override;
		void updateScene(Duration deltaTime) override;
		void renderScene(Renderer* renderer) override;
	public:
		[[nodiscard]] Window* getWindow() const;
	};
}
