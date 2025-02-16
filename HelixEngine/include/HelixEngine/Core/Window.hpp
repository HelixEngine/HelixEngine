#pragma once
#include <HelixEngine/Base/Object.hpp>
#include <SDL3/SDL.h>
#include <QtWidgets/qwidget.h>

#include "HelixEngine/Math/Vector2.hpp"

namespace helix
{
	class Window : public Object
	{
	public:
		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t width = 600, int32_t height = 600);
		explicit Window(std::u8string_view title = u8"HelixEngine",Vector2I32 size = {600,600});
	private:
		QWidget* qtWidget = nullptr;
	};
}
