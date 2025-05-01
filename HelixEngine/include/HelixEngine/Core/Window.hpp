#pragma once
#include <HelixEngine/Math/Vector2.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/BitOption.hpp>
#include <HelixEngine/Util/Object.hpp>
#include <HelixEngine/Util/Singleton.hpp>
#include <SDL3/SDL.h>

#include <HelixEngine/Node/Scene.hpp>

namespace helix::opengl
{
	class Renderer;
}

namespace helix
{
	class Window final : public Object
	{
		friend class opengl::Renderer;
	public:
		enum class Flag : uint32_t
		{
			// MaximumButton = 0b01,
			// MinimumButton = 0b10,
			// MinMaxButton = MaximumButton | MinimumButton,
			Default = 0,
		};

		struct Property
		{
			std::u8string title = u8"HelixEngine";
			Vector2I32 size = {600, 600};
			bool isResizable = true;
			bool isDisplay = true;
			GraphicsApi graphicsApi = GraphicsApi::Default;
			BitOption<Flag> flag = Flag::Default;
		};

		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t width = 600,
		                int32_t height = 600);
		explicit Window(std::u8string_view title = u8"HelixEngine", Vector2I32 size = {600, 600});
		explicit Window(const Property& property);
		~Window() override;

		void show() const;
		void hide() const;
		void setDisplay(bool isDisplay = true) const;
		[[nodiscard]] bool isDisplay() const;

		void setSize(Vector2I32 newSize) const;
		[[nodiscard]] Vector2I32 getSize() const;

		[[nodiscard]] SDL_Window* getSDLWindow() const;

		[[nodiscard]] const Color& getBackgroundColor() const;
		void setBackgroundColor(Color color);
	private:
		SDL_Window* sdlWindow = nullptr;
		Color backgroundColor;

		static void sdlError(std::u8string_view content);

		struct SDLInstance : Singleton<SDLInstance>
		{
			SDLInstance();
			~SDLInstance();
		};

		//渲染器
		Ref<Renderer> renderer;
	public:
		[[nodiscard]] const Ref<Renderer>& getRenderer() const;
	private:
		Ref<SceneBase> scene;
	public:
		void enter(Ref<SceneBase> scene);
	};
}
