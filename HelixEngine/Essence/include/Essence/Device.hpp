#pragma once
#include <vector>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/Singleton.hpp>
#include <Essence/Enum.hpp>
#include <Essence/Component/Component.hpp>

namespace essence
{
	class Device : public helix::Object
	{
		static inline auto graphicsApi = GraphicsApi::Vulkan;
	protected:
		struct Instance : helix::Singleton<Instance>
		{
			Instance();
			std::vector<helix::Ref<Device>> devices;
		};

		static inline std::vector<helix::Ref<Component>> globalComponents;
	public:
		static void setGraphicsApi(GraphicsApi graphicsApi);
		[[nodiscard]] static GraphicsApi getGraphicsApi();
		static const std::vector<helix::Ref<Device>>& getDevices();
		static inline std::vector<helix::Ref<component::Loader>> loaders;
		static void setComponentLoaders(std::vector<helix::Ref<component::Loader>> loaders);
		static std::vector<helix::Ref<Component>> getGlobalComponents();
	};
}
