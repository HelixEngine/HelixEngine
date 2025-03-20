#pragma once
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Math/Transform.hpp>

namespace helix
{
	class Node2D : public Object
	{
		friend class Scene;
	public:
		void addNode(Ref<Node2D> child);
	protected:
		virtual void onTick(Duration deltaTime)
		{
		}

		//后续要和自己的渲染做适配，现在先搭框架
		virtual void onRender()
		{
		}
	private:
		void update(Duration deltaTime);
		void render();
		std::vector<Ref<Node2D>> children;
		Transform2D transform;
	};

}
