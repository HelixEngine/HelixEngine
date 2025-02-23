#pragma once
#include <HelixEngine/Util/Time.hpp>
#include <QWidget>
#include <QTimer>

namespace helix
{
	class Window;
}

namespace helix::qt
{
	class Widget final : public QWidget
	{
		friend class Window;
		Q_OBJECT
		explicit Widget(Window* hxWindow);
		void sceneUpdate();
		std::unique_ptr<QTimer> timer;
		SteadyClock::TimePoint lastFrameTimePoint;
		Window* hxWindow{};
	};
}
