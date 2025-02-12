#include <SDL3/SDL.h>
#include <HelixEngine/Core/Window.hpp>

helix::Window::Window(const std::u8string_view title, const int32_t width, const int32_t height)
{
	sdlWindow = SDL_CreateWindow(reinterpret_cast<const char*>(title.data()), width, height, SDL_WINDOW_RESIZABLE);
}
