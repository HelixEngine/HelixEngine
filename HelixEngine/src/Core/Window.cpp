#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Core/QtWindow/Widget.hpp>

#include "HelixEngine/Core/Game.hpp"

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
	qWidget = std::make_unique<qt::Widget>();
	renderer = new Renderer(qWidget->winId());
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
}

helix::Window::~Window()
{
	updateThread.request_stop();
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

QWidget* helix::Window::getQWidget() const
{
	return qWidget.get();
}

void helix::Window::setParent(Window* parent) const
{
	qWidget->setParent(parent ? parent->getQWidget() : nullptr);
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
