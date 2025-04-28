#pragma once
#include <thread>
#include <HelixEngine/Enum.hpp>
#include <HelixEngine/Render/CommandQueue.hpp>
#include <HelixEngine/Util/BitOption.hpp>
#include <HelixEngine/Util/Color.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace helix
{
	using RenderList = CommandList<RenderCommand>;
	using RenderQueue = CommandQueue<RenderCommand>;

	class Renderer : public Object
	{
	public:
		~Renderer() override;
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;
	private:
		Ref<RenderQueue> queue = new RenderQueue;
	protected:
		std::jthread renderThread;
	};
}
