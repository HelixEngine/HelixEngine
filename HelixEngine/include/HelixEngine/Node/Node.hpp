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
	private:
		void update(Duration deltaTime);
		std::vector<Ref<Node2D>> children;
		Transform2D transform;
	};

}
