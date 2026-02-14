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

	Window::Window(const Property& property)
	{
		auto sSize = Vector2I32(property.size);
		sdlWindow = SDL_CreateWindow(
				reinterpret_cast<const char*>(property.title.data()),
				sSize.x, sSize.y,
				(property.isResizable ? SDL_WINDOW_RESIZABLE : 0) |
				SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY |
				(property.graphicsApi == GraphicsApi::OpenGL ? SDL_WINDOW_OPENGL : 0));

		SDL_SetPointerProperty(SDL_GetWindowProperties(sdlWindow), sdlWindowPointerProperty.data(), this);


		if (sdlWindow == nullptr)
		{
			sdlError(u8"创建SDL窗口失败");
			return;
		}

		allWindows.emplace_back(this);

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
		renderer->graphicsApi = property.graphicsApi;

		renderer->window = this;
		graphicsApi = property.graphicsApi;
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

	void Window::setSize(Vector2UI32 newSize) const
	{
		auto sSize = Vector2I32(newSize);
		if (!SDL_SetWindowSize(sdlWindow, sSize.x, sSize.y))
			sdlError(u8"设置SDL窗口大小失败");
	}

	Vector2UI32 Window::getSize() const
	{
		Vector2I32 size;
		SDL_GetWindowSize(sdlWindow, &size.x, &size.y);
		return Vector2UI32(size);
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
		return graphicsApi;
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
		for (auto& window: allWindows)
		{
			window->updateThread = std::jthread{[=](const std::stop_token& token)
			{
				window->updateThreadFunc(token);
			}};
			window->renderer->startRun();
		}

		std::vector<std::thread> readyThreads(Window::allWindows.size());
		for (size_t i = 0; i < readyThreads.size(); ++i)
		{
			readyThreads[i] = std::thread([i] { Window::allWindows[i]->getRenderer()->readyRender(); });
		}

		for (auto& readyThread: readyThreads)
		{
			readyThread.join();
		}

		for (auto& window: Window::getAllWindows())
		{
			Renderer* renderer = window->getRenderer();
			renderer->renderThread = std::jthread([renderer](const std::stop_token& token)
			{
				renderer->renderThreadFunc(token);
			});
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
