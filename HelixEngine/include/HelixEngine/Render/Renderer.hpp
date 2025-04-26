#pragma once
#include <QWidget>
#include <thread>
#include <HelixEngine/Enum.hpp>
#include <HelixEngine/Render/RenderQueue.hpp>
#include <HelixEngine/Util/BitOption.hpp>
#include <HelixEngine/Util/Color.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace helix
{
	class Window;
	class Renderer;

	struct RenderThreadInstance
	{
		Renderer* renderer = nullptr;
		std::shared_ptr<QWidget> qWidget = nullptr;
	};

	class RenderInstance : public Object
	{
		friend class Renderer;
	protected:
		[[nodiscard]] static bool queryFeature(RenderFeature feature);
		static const Ref<RenderCommandBuffer>& swapBackBuffer(const Ref<RenderQueue>& renderQueue);
		virtual std::function<void(RenderThreadInstance)> getThreadFunc() = 0;
	};

	class Renderer final : public Object
	{
		friend class RenderInstance;
		static inline Ref<RenderInstance> renderInstance;
		static inline BitOption<RenderFeature> feature = RenderFeature::Default;
	public:
		static void setRenderFeature(BitOption<RenderFeature> feature);
		static BitOption<RenderFeature> getRenderFeature();
		static void setRenderInstance(Ref<RenderInstance> instance);
		Renderer();
		explicit Renderer(Window* window);
		~Renderer() override;

		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;
		void closeRenderThread();
	private:
		std::jthread renderThread;
		Ref<RenderQueue> renderQueue = new RenderQueue;
		Window* window;
	};
}

namespace helix_render2
{
	using namespace helix;
	using namespace helix_cmd;

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
