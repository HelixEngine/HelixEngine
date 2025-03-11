#pragma once
#include <vector>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Singleton.hpp>
#include <Essence/Enum.hpp>

namespace essence
{
	class Device : public helix::Object
	{
		struct Instance : helix::Singleton<Instance>
		{
			Instance();
			std::vector<helix::Ref<Device>> devices;
		};

		static inline auto api = GraphicsApi::Vulkan;
	public:
		static void setGraphicsApi(GraphicsApi api);
		[[nodiscard]] static GraphicsApi graphicsApi();
	};
}
