#pragma once
#include <HelixEngine/Node/Node.hpp>

namespace helix::qt
{
	class Widget;
}

namespace helix
{
	class Scene : public Node2D
	{
		friend class Window;
	protected:
		virtual void onEnter()
		{
		}
	private:
		void updateScene(Duration deltaTime);
		void renderScene();
	};
}
