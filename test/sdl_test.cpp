#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref renderer = new opengl::Renderer{window.get()};

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
