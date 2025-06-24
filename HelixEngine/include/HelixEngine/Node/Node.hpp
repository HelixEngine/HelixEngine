#pragma once
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Util/IntrusiveList.hpp>

namespace helix
{
	class NodeBase : public Object
	{
		friend class Window;
		friend class Scene2D;
	protected:
		virtual void update(Duration deltaTime) = 0;
		virtual void render(Renderer* renderer) = 0;
	private:
		//For Scene
		virtual void setWindow(Window* window)
		{
		}

		virtual void updateScene(Duration deltaTime)
		{
		}

		virtual void renderScene(Renderer* renderer)
		{
		}
	};

	class Node2D : public NodeBase, protected IntrusiveNode<Ref<Node2D>>
	{
		friend class IntrusiveList<Ref<Node2D>>;
		friend class Scene2D;
	protected:
		void update(Duration deltaTime) override;
		void render(Renderer* renderer) override;
	private:
		Transform2D transform;

		IntrusiveList<Ref<Node2D>> children;
		Node2D* parent = nullptr;

		void updateAll(Duration deltaTime);
		void renderAll(Renderer* renderer);
	public:
		void addChild(const Ref<Node2D>& child);
		void removeChildImmediate(Ref<Node2D>& child);
		Ref<Node2D> findChild(std::u8string_view name);
		[[nodiscard]] const IntrusiveList<Ref<Node2D>>& getAllChildren() const;

		[[nodiscard]] Node2D* getParent() const;
		[[nodiscard]] const Transform2D& getTransform() const;
	};
}
