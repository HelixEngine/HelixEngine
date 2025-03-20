#pragma once
#include <mutex>
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
		void sceneRender();
		std::unique_ptr<QTimer> updateTimer;
		std::unique_ptr<QTimer> renderTimer;
		SteadyClock::TimePoint lastFrameTimePoint;
		Window* hxWindow{};
	};
}
