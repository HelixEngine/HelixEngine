#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>

#include "HelixEngine/Core/Game.hpp"
#include "HelixEngine/Util/Logger.hpp"

helix::Window::Window(const std::u8string_view title, const int32_t width, const int32_t height) :
	Window(title, Vector2I32{width, height})
{
}

helix::Window::Window(const std::u8string_view title, const Vector2I32 size) :
	Window(Property{.title = title.data(), .size = size})
{
}

helix::Window::Window(const Property& property)
{
	Game::Instance::getInstance();
	qWidget = std::make_shared<qt::Widget>(this);
	setProperty(property);
	updateThread = std::jthread{[this](const std::stop_token& token)
	{
		auto now = SteadyClock::now();
		auto last = now;
		while (!token.stop_requested())
		{
			const auto temp = scene;
			now = SteadyClock::now();
			temp->updateScene(now - last);
			temp->renderScene(); // 后续把Renderer扔进去，给它用
			last = now;
		}
	}};
	qWidget->show();
	renderer = new Renderer(this);
}

helix::Window::~Window()
{
	closeUpdateThread();
}

void helix::Window::setSize(const Vector2I32 newSize) const
{
	qWidget->resize(QSize{newSize});
}

// void helix::Window::setFixedSize(const Vector2I32 fixedSize) const
// {
// 	qWidget->setFixedSize(QSize{fixedSize});
// }

void helix::Window::setFixedSize(const bool isFixed) const
{
	if (isFixed)
	{
		qWidget->setMaximumSize(qWidget->size());
		qWidget->setMinimumSize(qWidget->size());
		return;
	}

	qWidget->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
	qWidget->setMinimumSize(0, 0);
}

helix::Vector2I32 helix::Window::getSize() const
{
	return Vector2I32{qWidget->size().width(), qWidget->size().height()};
}

std::shared_ptr<QWidget> helix::Window::getQWidget() const
{
	return qWidget;
}

void helix::Window::setParent(Window* parent) const
{
	qWidget->setParent(parent ? parent->getQWidget().get() : nullptr);
	qWidget->setProperty(qtParentPropertyName, QVariant::fromValue(parent));
}

helix::Window* helix::Window::getParent() const
{
	return qWidget->parent()->property("HelixEngine.Window:Parent").value<Window*>();
}

void helix::Window::setFlag(BitOption<Flag> flag) const
{
	Qt::WindowFlags flags = qWidget->windowFlags();
	flags.setFlag(Qt::WindowMaximizeButtonHint, flag.getItem(Flag::MaximumButton));
	flags.setFlag(Qt::WindowMinimizeButtonHint, flag.getItem(Flag::MinimumButton));
	qWidget->setWindowFlags(flags);
}

helix::BitOption<helix::Window::Flag> helix::Window::getFlag() const
{
	BitOption<Flag> flag;
	flag.setItem(Flag::MaximumButton, qWidget->windowFlags().testFlag(Qt::WindowMaximizeButtonHint));
	flag.setItem(Flag::MinimumButton, qWidget->windowFlags().testFlag(Qt::WindowMinimizeButtonHint));
	return flag;
}

void helix::Window::setProperty(const Property& property) const
{
	setFlag(property.flag);
	setParent(property.parent);

	setSize(property.size);
	setTitle(property.title);

	setDisplay(property.isDisplay);
}

helix::Window::Property helix::Window::getProperty() const
{
	Property property;
	property.parent = getParent();
	property.size = getSize();
	property.flag = getFlag();
	property.isDisplay = isDisplay();
	property.title = getTitle();
	return property;
}

void helix::Window::setDisplay(const bool isDisplay) const
{
	qWidget->setHidden(!isDisplay);
}

bool helix::Window::isDisplay() const
{
	return !qWidget->isHidden();
}

void helix::Window::setTitle(const std::u8string_view newTitle) const
{
	qWidget->setWindowTitle(newTitle.data());
}

std::u8string helix::Window::getTitle() const
{
	auto title = qWidget->windowTitle().toUtf8();
	return std::u8string{title.begin(), title.end()};
}

helix::Ref<helix::Renderer> helix::Window::getRenderer()
{
	return renderer;
}

void helix::Window::enter(Ref<Scene> scene)
{
	scene->updateWindowPtr(this);
	this->scene = std::move(scene);
}

const helix::Ref<helix::Scene>& helix::Window::getScene() const
{
	return scene;
}

void helix::Window::closeUpdateThread()
{
	updateThread.request_stop();
	if (updateThread.joinable())
		updateThread.join();
}

namespace helix_sdl3
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
				SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL);

		if (sdlWindow == nullptr)
		{
			sdlError(u8"创建SDL窗口失败");
			return;
		}

		if (property.isDisplay)
			show();
	}

	Window::~Window()
	{
		SDL_DestroyWindow(sdlWindow);
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

	void Window::sdlError(std::u8string_view content)
	{
		Logger::error(content, u8": [", std::u8string(reinterpret_cast<const char8_t*>(SDL_GetError())), u8"]");
	}

	Window::SDLInstance::SDLInstance()
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			sdlError(u8"SDL初始化失败");
		}
	}

	Window::SDLInstance::~SDLInstance()
	{
		SDL_Quit();
	}
}
