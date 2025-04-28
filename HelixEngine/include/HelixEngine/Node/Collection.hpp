#pragma once
#include <HelixEngine/Node/Node.hpp>
#include <boost/intrusive/list.hpp>

namespace helix
{
	class Collection2D final : public Node2D
	{
		IntrusiveList hooks;
	public:
		void addNode(const Ref<Node2D>& node);
		[[nodiscard]] std::vector<Node2D*> getNodes() const;
	private:
		void update(Duration deltaTime) override;
		void render(Renderer* renderer) override;
	};
}
