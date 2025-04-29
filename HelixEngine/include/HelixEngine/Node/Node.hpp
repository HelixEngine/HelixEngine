#pragma once
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <boost/intrusive/list.hpp>
#include <ranges>

namespace helix
{
	class Node2D : public Object
	{
	protected:
		virtual void update(Duration deltaTime) = 0;
		virtual void render(Renderer* renderer) = 0;
	private:
		Transform2D transform;

		struct IntrusiveHook : boost::intrusive::list_base_hook<>
		{
			Ref<Node2D> node;
		} intrusiveHook;

		using IntrusiveList = boost::intrusive::list<IntrusiveHook>;
		IntrusiveList children;
	public:
		//等待测试
		void addNode(const Ref<Node2D>& child);
		void removeNode(const Ref<Node2D>& child);
	};
}
