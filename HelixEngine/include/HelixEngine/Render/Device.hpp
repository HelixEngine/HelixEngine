#pragma once
#include <memory>
#include <HelixEngine/Base/Singleton.hpp>
#include <HelixEngine/Render/GraphicsDevice/GraphicsDevice.hpp>

namespace helix::render
{
	class Device final
	{
	public:
		enum class GraphicsAPI
		{
			Vulkan,
		};
	private:
		static inline auto graphicsAPI = GraphicsAPI::Vulkan;
	public: //暂时
		struct Instance final : Singleton<Instance>
		{
			Instance();
			std::unique_ptr<graphics_device::GraphicsDevice> graphicsDevice;
		};
	};
}
