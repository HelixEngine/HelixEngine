#pragma once
#include <HelixEngine/Base/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>

namespace helix
{
	class Node : public Object
	{
	public:
		void addNode(Ref<Node> child);
	protected:
		virtual void onTick(Duration deltaTime)
		{
		}
	private:
		std::vector<Ref<Node>> children;
	};

}
