#include <HelixEngine/Core/QtWindow/Widget.hpp>
#include <QTimer>
#include <HelixEngine/Util/Time.hpp>

helix::qt::Widget::Widget() :
	timer(std::make_unique<QTimer>())
{
	connect(timer.get(), SIGNAL(QTimer::timeout), this, SLOT(update));
	timer->start(0ms);
}

void helix::qt::Widget::update()
{

}
