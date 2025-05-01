#pragma once
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Util/IntrusiveList.hpp>

namespace helix
{
	class Node2D : public Object, protected IntrusiveNode<Ref<Node2D>>
	{
		friend class IntrusiveList<Ref<Node2D>>;
	protected:
		virtual void update(Duration deltaTime);
		virtual void render(Renderer* renderer);
	private:
		Transform2D transform;

		IntrusiveList<Ref<Node2D>> children;
	public:
		void addChild(Ref<Node2D>& child);
		void removeChild(Ref<Node2D>& child);
		[[nodiscard]] IntrusiveList<Ref<Node2D>> getAllChildren() const;
	};
}
