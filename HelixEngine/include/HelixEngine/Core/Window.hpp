#pragma once
#include <HelixEngine/Base/Object.hpp>
#include <SDL3/SDL.h>
#include <QtWidgets/qwidget.h>

namespace helix
{
	class Window : public Object
	{
	public:
		~Window() override { puts("WOW"); }
		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t width = 600, int32_t height = 600);
		//private:
		SDL_Window* sdlWindow = nullptr;
		QWidget* qtWidget = nullptr;
	};
}
