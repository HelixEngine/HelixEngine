#pragma once
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <boost/intrusive/list.hpp>

namespace helix
{
	class Node2D : public Object
	{
		friend class Collection2D;
	protected:
		virtual void update(Duration deltaTime) = 0;
		virtual void render(Renderer* renderer) = 0;
	private:
		Transform2D transform;

		struct IntrusiveHook : boost::intrusive::list_base_hook<>
		{
			explicit IntrusiveHook(Node2D* node) :
				node(node)
			{
			}

			Node2D* node{};
		};

		using IntrusiveList = boost::intrusive::list<IntrusiveHook>;

		IntrusiveHook intrusiveHook{this};
	};
}
