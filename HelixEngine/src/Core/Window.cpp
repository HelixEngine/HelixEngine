#include <HelixEngine/Core/Game.hpp>
#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

namespace helix
{
	Window::Window(std::u8string_view title, uint32_t width, uint32_t height) :
		Window(title, Vector2UI32{width, height})
	{

	}

	Window::Window(std::u8string_view title, Vector2UI32 size) :
		Window(Property{.title = title.data(), .size = size})
	{

	}

	Window::Window(Property nProperty) : property(std::move(nProperty))
	{
		allWindows.emplace_back(this);

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
		renderer->graphicsApi = property.graphicsApi;
		renderer->window = this;
	}

	Window::~Window()
	{
		destroy();
	}

	void Window::show()
	{
		property.flag.setItem(Flag::Visible,true);
		if (sdlWindow)
			if (!SDL_ShowWindow(sdlWindow))
				sdlError(u8"显示SDL窗口失败");
	}

	void Window::hide()
	{
		property.flag.setItem(Flag::Visible,false);
		if (sdlWindow)
			if (!SDL_HideWindow(sdlWindow))
				sdlError(u8"隐藏SDL窗口失败");
	}

	void Window::setVisible(bool isVisible)
	{
		if (isVisible)
			show();
		else
			hide();
	}

	bool Window::isVisible() const
	{
		return property.flag.getItem(Flag::Visible);
	}

	void Window::setSize(Vector2UI32 newSize)
	{
		property.size = newSize;
		if (sdlWindow)
		{
			auto sSize = Vector2I32(newSize);
			if (!SDL_SetWindowSize(sdlWindow, sSize.x, sSize.y))
				sdlError(u8"设置SDL窗口大小失败");
		}
	}

	Vector2UI32 Window::getSize() const
	{
		return property.size;
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
			updateThread.request_stop();
			if (updateThread.joinable())
				updateThread.join();
			auto temp = sdlWindow;
			sdlWindow = nullptr;
			//由于之后将allWindows改为std::vector<Ref<Window>>后，这条代码可能会触发析构函数，导致再次调用destroy，需要将作为flag的sdlWindow先设为nullptr
			allWindows.erase(std::ranges::find(allWindows, Ref(this)));
			SDL_DestroyWindow(temp);
		}
	}

	const std::vector<Ref<Window>>& Window::getAllWindows()
	{
		return allWindows;
	}

	GraphicsApi Window::getGraphicsApi() const
	{
		return property.graphicsApi;
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

	void Window::start()
	{
		for (auto& window: allWindows)
		{
			window->create();
		}
	}

	void Window::create()
	{
		auto sSize = Vector2I32(property.size);
		sdlWindow = SDL_CreateWindow(
				reinterpret_cast<const char*>(property.title.data()),
				sSize.x, sSize.y,
				(property.flag.getItem(Flag::Resizable) ? SDL_WINDOW_RESIZABLE : 0) |
				(property.flag.getItem(Flag::MinimumButton) ? SDL_WINDOW_MINIMIZED : 0) |
				(property.flag.getItem(Flag::MaximumButton) ? SDL_WINDOW_MAXIMIZED : 0) |
				SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY |
				(property.graphicsApi == GraphicsApi::OpenGL ? SDL_WINDOW_OPENGL : 0));

		if (!sdlWindow)
		{
			sdlError(u8"创建SDL窗口失败");
			return;
		}

		SDL_SetPointerProperty(SDL_GetWindowProperties(sdlWindow), sdlWindowPointerProperty.data(), this);

		if (property.graphicsApi == GraphicsApi::OpenGL)
		{
			//创建 OpenGL 上下文
			auto glRenderer = reinterpret_cast<opengl::Renderer*>(renderer.get());
			glRenderer->createSDLContext();
		}

		updateThread = std::jthread{[this](const std::stop_token& token)
		{
			updateThreadFunc(token);
		}};

		renderer->renderThread = std::jthread([this](const std::stop_token& token)
		{
			renderer->renderThreadFunc(token);
		});

		if (isVisible())
			show();
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
			if (auto curr = scene)
			{
				auto now = SteadyClock::now();
				curr->updateScene(now - lastUpdateTime);
				lastUpdateTime = now;

				curr->renderScene(getRenderer());
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
