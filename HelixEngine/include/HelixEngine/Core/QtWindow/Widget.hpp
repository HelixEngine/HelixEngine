#pragma once
#include <QWidget>

namespace helix::qt
{
	class Widget final : public QWidget
	{
		friend class Window;
		Q_OBJECT
		Widget();
		void update();
		std::unique_ptr<QTimer> timer;
	};
}
