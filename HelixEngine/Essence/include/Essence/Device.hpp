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
		std::vector<helix::Ref<Component>> components;

		template<BasedComponent ComponentType>
		static void findComponent(const std::vector<helix::Ref<Component>>& components,
		                          helix::Ref<ComponentType>& component)
		{
			auto it = std::ranges::find_if(components, [](const helix::Ref<Component>& component)
			{
				return component->getComponentName() == ComponentType::componentName;
			});

			if (it != components.end())
				component = reinterpret_cast<ComponentType*>(it->get());
		}
	public:
		static void setGraphicsApi(GraphicsApi graphicsApi);
		[[nodiscard]] static GraphicsApi getGraphicsApi();
		static const std::vector<helix::Ref<Device>>& getDevice();
		static inline std::vector<helix::Ref<component::Loader>> loaders;
		static void setComponentLoader(std::vector<helix::Ref<component::Loader>> loaders);
		static const std::vector<helix::Ref<Component>>& getAllGlobalComponent();

		template<BasedComponent ComponentType>
		static void findGlobalComponent(helix::Ref<ComponentType>& component)
		{
			findComponent(getAllGlobalComponent(), component);
		}

		const std::vector<helix::Ref<Component>>& getAllComponent();

		template<BasedComponent ComponentType>
		void findComponent(helix::Ref<ComponentType>& component)
		{
			findComponent(getAllComponent(), component);
		}

		template<BasedComponent ComponentType>
		static void findDeviceComponent(helix::Ref<ComponentType>& component)
		{
			for (const auto& device: getDevice())
				findComponent(device->getAllComponent(), component);
		}
	};
}
