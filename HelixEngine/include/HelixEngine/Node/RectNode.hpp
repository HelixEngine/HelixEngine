#pragma once
#include <HelixEngine/Node/Node.hpp>

namespace helix
{
	class RectNode : public Node2D
	{
	protected:
		void onRender() override;
	};
}
