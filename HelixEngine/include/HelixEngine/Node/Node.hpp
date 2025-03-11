#pragma once
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>

namespace helix
{
	class Node : public Object
	{
		friend class Scene;
	public:
		void addNode(Ref<Node> child);
	protected:
		virtual void onTick(Duration deltaTime)
		{
		}
	private:
		void update(Duration deltaTime);
		std::vector<Ref<Node>> children;
	};

}
