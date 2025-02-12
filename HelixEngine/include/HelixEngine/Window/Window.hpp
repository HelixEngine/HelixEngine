#pragma once
#include <string>
#include <SDL3/SDL.h>

namespace helix
{
	class Window
	{
	public:
		explicit Window(std::u8string_view title = u8"HelixEngine", int32_t width = 600, int32_t height = 600);
	private:
		SDL_Window* sdlWindow = nullptr;
	};
}
