#pragma once
#include <functional>
#include <thread>
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
		friend class Window;
		friend class Game;
	public:
		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;
		[[nodiscard]] Window* getWindow() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;
	private:
		Ref<RenderQueue> queue = new RenderQueue;
		std::jthread renderThread;
		Window* window = nullptr;
	protected:
		using RenderThreadFunc = std::function<void(const std::stop_token&)>;
		void startRenderThread(RenderThreadFunc func);
	private:
		//Game run

		virtual void startRun() = 0;
	};
}
