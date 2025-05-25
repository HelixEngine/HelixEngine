#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Util/Logger.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

extern helix::RenderPipeline* helix_test_render_pipeline;

namespace helix
{
	Window::Window(std::u8string_view title, int32_t width, int32_t height) :
		Window(title, Vector2I32{width, height})
	{

	}

	Window::Window(std::u8string_view title, Vector2I32 size) :
		Window(Property{.title = title.data(), .size = size})
	{

	}

	Window::Window(const Property& property)
	{
		sdlWindow = SDL_CreateWindow(
				reinterpret_cast<const char*>(property.title.data()),
				property.size.x, property.size.y,
				(property.isResizable ? SDL_WINDOW_RESIZABLE : 0) |
				SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY |
				(property.graphicsApi == GraphicsApi::OpenGL ? SDL_WINDOW_OPENGL : 0));

		SDL_SetPointerProperty(SDL_GetWindowProperties(sdlWindow), sdlWindowPointerProperty.data(), this);


		if (sdlWindow == nullptr)
		{
			sdlError(u8"创建SDL窗口失败");
			return;
		}

		allWindows.push_back(this);

		if (property.isDisplay)
			show();

		//设置渲染器
		switch (property.graphicsApi)
		{
			case GraphicsApi::OpenGL:
				renderer = new opengl::Renderer;
				break;
			default:
				Logger::warning(u8"Window: 不支持的GraphicsApi，默认使用OpenGL");
				renderer = new opengl::Renderer;
				break;
		}

		renderer->window = this;
	}

	Window::~Window()
	{
		destroy();
	}

	void Window::show() const
	{
		if (!SDL_ShowWindow(sdlWindow))
			sdlError(u8"显示SDL窗口失败");
	}

	void Window::hide() const
	{
		if (!SDL_HideWindow(sdlWindow))
			sdlError(u8"隐藏SDL窗口失败");
	}

	void Window::setDisplay(bool isDisplay) const
	{
		if (isDisplay)
			show();
		else
			hide();
	}

	bool Window::isDisplay() const
	{
		return SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_HIDDEN;
	}

	void Window::setSize(Vector2I32 newSize) const
	{
		if (!SDL_SetWindowSize(sdlWindow, newSize.x, newSize.y))
			sdlError(u8"设置SDL窗口大小失败");
	}

	Vector2I32 Window::getSize() const
	{
		Vector2I32 size;
		SDL_GetWindowSize(sdlWindow, &size.x, &size.y);
		return size;
	}

	SDL_Window* Window::getSDLWindow() const
	{
		return sdlWindow;
	}

	const Color& Window::getBackgroundColor() const
	{
		return backgroundColor;
	}

	void Window::setBackgroundColor(Color color)
	{
		backgroundColor = color;
	}

	void Window::destroy()
	{
		if (sdlWindow)
		{
			renderer->renderThread.request_stop();
			renderer->renderThread.join();
			allWindows.erase(std::ranges::find(allWindows, this));
			SDL_DestroyWindow(sdlWindow);
			sdlWindow = nullptr;
		}
	}

	const std::vector<Window*>& Window::getAllWindows()
	{
		return allWindows;
	}

	void Window::sdlError(std::u8string_view content)
	{
		Logger::error(content, u8": [", std::u8string(reinterpret_cast<const char8_t*>(SDL_GetError())), u8"]");
	}

	void Window::SDLInit()
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			sdlError(u8"SDL初始化失败");
		}
	}

	void Window::SDLQuit()
	{
		SDL_Quit();
	}

	void Window::startRun()
	{
		for (auto window: allWindows)
		{
			window->updateThread = std::jthread{[=](const std::stop_token& token)
			{
				window->updateThreadFunc(token);
			}};
			window->renderer->startRun();
		}
	}

	const Ref<Renderer>& Window::getRenderer() const
	{
		return renderer;
	}

	void Window::updateThreadFunc(const std::stop_token& token) const
	{
		SteadyClock::TimePoint lastUpdateTime = SteadyClock::now();
		while (!token.stop_requested())
		{
			getRenderer()->begin(getBackgroundColor());
			auto curr = scene;
			if (curr)
			{
				auto now = SteadyClock::now();
				curr->update(now - lastUpdateTime);
				lastUpdateTime = now;

				getRenderer()->setRenderPipeline(helix_test_render_pipeline);
				curr->render(getRenderer());
			}
			getRenderer()->end();
		}
	}

	void Window::enter(const Ref<Scene2D>& newScene)
	{
		enterAsNodeBase(newScene);
	}

	void Window::enterAsNodeBase(const Ref<NodeBase>& newScene)
	{
		if (scene)
			scene->setWindow(nullptr);
		newScene->setWindow(this);
		scene = newScene;
	}
}
