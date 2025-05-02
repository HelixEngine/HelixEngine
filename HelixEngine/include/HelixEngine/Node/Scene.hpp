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
	public:
		[[nodiscard]] Window* getWindow() const;
	};
}
