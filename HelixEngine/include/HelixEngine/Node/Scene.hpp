#pragma once
#include <HelixEngine/Node/Node.hpp>

namespace helix
{
	class Window;
	//思考一下后续怎么搞

	class SceneBase : public Node2D
	{
		friend class Window;
		Window* window{};
	public:
		// virtual ~SceneBase() = default;
		[[nodiscard]] Window* getWindow() const;
		// protected:
		// 	virtual void updateScene() = 0;
		// 	virtual void renderScene() = 0;
	};

	class Scene2D final : public SceneBase
	{
		//void render(Renderer* renderer) override;
	};
}
