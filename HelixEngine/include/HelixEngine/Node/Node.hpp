#pragma once
#include <ranges>
#include <HelixEngine/Math/Transform.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Time.hpp>
#include <boost/intrusive/list.hpp>
#include <HelixEngine/Util/Logger.hpp>

namespace helix
{
	class Node2D : public Object
	{
	protected:
		virtual void update(Duration deltaTime);
		virtual void render(Renderer* renderer);
	private:
		Transform2D transform;

		struct IntrusiveHook : boost::intrusive::list_base_hook<>
		{
			Ref<Node2D> node;
			~IntrusiveHook() { node.reset(); }
		} intrusiveHook;

		using IntrusiveList = boost::intrusive::list<IntrusiveHook>;
		IntrusiveList children;
	public:
		using ChildrenView = std::ranges::transform_view<
			std::ranges::ref_view<const boost::intrusive::list<IntrusiveHook>>, std::function<Ref<Node2D>(
					const IntrusiveHook&)>>;
		//等待测试
		void addChild(const Ref<Node2D>& child);
		void removeChild(const Ref<Node2D>& child);
		[[nodiscard]] ChildrenView getAllChildren() const;

		~Node2D() override
		{
			children.clear();
			Logger::info(u8"Node destroyed:", getName());
		}
	};
}
