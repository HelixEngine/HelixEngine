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
	class Renderer;

	struct RenderThreadInstance
	{
		Renderer* renderer = nullptr;
		WId windowId{};
	};

	class RenderInstance : public Object
	{
		friend class Renderer;
	protected:
		[[nodiscard]] static bool queryFeature(RenderFeature feature);
		static const helix::Ref<helix::RenderCommandBuffer>& swapBackBuffer(const Ref<RenderQueue>& renderQueue);
		virtual void threadFunc(RenderThreadInstance) = 0;
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
		explicit Renderer(WId windowId);
		~Renderer() override;

		[[nodiscard]] const Ref<RenderQueue>& getRenderQueue() const;

		//Render Command

		void begin(Color clearColor) const;
		void end() const;
	private:
		std::jthread renderThread;
		Ref<RenderQueue> renderQueue = new RenderQueue;
	};
}
