#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>
#include <glad/glad.h>

using namespace helix;

int main()
{
	Ref window = new helix_sdl3::Window{u8"Hello, HelixEngine", {800, 600}};
	// auto ctx = new helix_render2::opengl::RenderContext;
	// ctx->sdlGlContext = SDL_GL_CreateContext(window->getSDLWindow());
	// gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
	// window->context = ctx;
	Ref renderer = new helix_render2::opengl::Renderer{window.get()};

	//写一下SDL的main loop
	SDL_Event event;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
				default: ;
			}

			renderer->begin(Color::Red);
			renderer->end();
		}
	}
}
