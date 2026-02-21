#pragma once
#include <HelixEngine/Math/Vector.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Util/BitOption.hpp>
#include <HelixEngine/Util/Object.hpp>
#include <HelixEngine/Util/Singleton.hpp>
#include <HelixEngine/Node/Scene.hpp>
#include <HelixEngine/Enum.hpp>
#include <SDL3/SDL.h>

namespace helix::opengl
{
	class Renderer;
}

namespace helix
{

	class Window final : public Object
	{
		friend class Game;
		friend class opengl::Renderer;
	public:
		enum class Flag : uint32_t
		{
			MaximumButton = 0b01,
			MinimumButton = 0b10,
			MinMaxButton = MaximumButton | MinimumButton,
			Resizable = 0b100,
			Visible = 0b1000,
			Default = Resizable | Visible,
		};

		struct Property
		{
			std::u8string title = u8"HelixEngine";
			Vector2UI32 size = {600, 600};
			GraphicsApi graphicsApi = GraphicsApi::Default;
			BitOption<Flag> flag = Flag::Default;
		};

		explicit Window(std::u8string_view title, uint32_t width, uint32_t height);
		explicit Window(std::u8string_view title = u8"HelixEngine", Vector2UI32 size = {600, 600});
		explicit Window(Property nProperty);
		~Window() override;

		void show();
		void hide();
		void setVisible(bool isVisible = true);
		[[nodiscard]] bool isVisible() const;

		void setSize(Vector2UI32 newSize);
		[[nodiscard]] Vector2UI32 getSize() const;

		[[nodiscard]] SDL_Window* getSDLWindow() const;

		[[nodiscard]] const Color& getBackgroundColor() const;
		void setBackgroundColor(Color color);
		static const std::vector<Ref<Window>>& getAllWindows();
		[[nodiscard]] GraphicsApi getGraphicsApi() const;
	private:
		void destroy();
		static constexpr std::string_view sdlWindowPointerProperty = "HelixEngine:Window";
		SDL_Window* sdlWindow = nullptr;
		Color backgroundColor;
		Property property;

		static void sdlError(std::u8string_view content);

		static void SDLInit();
		static void SDLQuit();

		static inline std::vector<Ref<Window>> allWindows;
		static void start();

		void create();

		//渲染器
		Ref<Renderer> renderer;
	public:
		[[nodiscard]] const Ref<Renderer>& getRenderer() const;
	private:
		Ref<NodeBase> scene;
		std::jthread updateThread;
		void updateThreadFunc(const std::stop_token& token);
	public:
		void enter(const Ref<Scene2D>& newScene);
	private:
		void enterAsNodeBase(const Ref<NodeBase>& newScene);
	};
}
