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
		Q_OBJECT
	public:
		explicit Widget(Window* window);
		Window* window = nullptr;
		void closeEvent(QCloseEvent* event) override;
	};
}
