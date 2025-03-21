#pragma once
#include <mutex>
#include <HelixEngine/Util/Time.hpp>
#include <HelixEngine/Node/Scene.hpp>
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

		~Widget() override
		{
			std::lock_guard lock{mtx};
			updateThread.request_stop();
			renderThread.request_stop();
		}

		void sceneUpdate();
		void sceneRender();
		std::unique_ptr<QTimer> updateTimer;
		std::unique_ptr<QTimer> renderTimer;
		SteadyClock::TimePoint lastFrameTimePoint;
		Window* hxWindow{};
		std::mutex mtx;

		std::jthread updateThread{[this](const std::stop_token& token)
		{
			while (!token.stop_requested())
			{
				std::lock_guard lock{mtx};
				sceneUpdate();
			}
		}};

		std::jthread renderThread{[this](const std::stop_token& token)
		{
			while (!token.stop_requested())
			{
				std::lock_guard lock{mtx};
				sceneRender();
			}
		}};
	};
}
