#pragma once
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Util/IntrusiveList.hpp>

namespace helix
{
	// class NodeBase : public Object
	// {
	// 	virtual void update(Duration deltaTime) = 0;
	// 	virtual void render(Renderer* renderer) = 0;
	// };

	class Node2D : public Object, protected IntrusiveNode<Ref<Node2D>>
	{
		friend class IntrusiveList<Ref<Node2D>>;
	protected:
		virtual void update(Duration deltaTime);
		virtual void render(Renderer* renderer);
	private:
		Transform2D transform;

		IntrusiveList<Ref<Node2D>> children;
		Node2D* parent = nullptr;
	public:
		void addChild(Ref<Node2D>& child);
		void removeChildImmediate(Ref<Node2D>& child);
		Ref<Node2D> findChild(std::u8string_view name);
		[[nodiscard]] const IntrusiveList<Ref<Node2D>>& getAllChildren() const;

		[[nodiscard]] Node2D* getParent() const;
		[[nodiscard]] const Transform2D& getTransform() const;
	};
}
