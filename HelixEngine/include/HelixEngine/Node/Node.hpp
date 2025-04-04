#pragma once
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>

namespace helix
{
	class Window;

	class Node2D : public Object
	{
		friend class Scene;
		friend class Window;
	public:
		void addNode(Ref<Node2D> child);
		[[nodiscard]] Window* getWindow() const;
		[[nodiscard]] Node2D* getParent() const;
		[[nodiscard]] Scene* getScene() const;
		[[nodiscard]] Ref<Renderer> getRenderer() const;
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
		Node2D* parent = nullptr;
		Window* window = nullptr;
		void updateWindowPtr(Window* window);
	};

}
