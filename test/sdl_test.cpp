#include <HelixEngine/HelixEngine.hpp>
#include <glad/glad.h>
#include <SDL3/SDL_opengl.h>

int main()
{
	helix::Ref window = new helix_sdl3::Window{u8"Hello, HelixEngine", {800, 600}};

	auto sdlWindow = window->getSDLWindow();
	SDL_GLContext context = SDL_GL_CreateContext(sdlWindow);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

	//写一下SDL的main loop
	SDL_Event event;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				isRunning = false;
				break;
			}
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			{
				isRunning = false;
				break;
			}
		}
	}
}
