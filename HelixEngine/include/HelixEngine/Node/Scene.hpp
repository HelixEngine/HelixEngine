#pragma once
#include <HelixEngine/Node/Node.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>

namespace helix::qt
{
	class Widget;
}

namespace helix
{
	class Scene : public Node2D
	{
		friend class qt::Widget;
	protected:
		virtual void onEnter()
		{
		}
	private:
		void updateScene(Duration deltaTime);
		void renderScene();
	};
}
