#include <HelixEngine/HelixEngine.hpp>
#include <glad/glad.h>
#include <HelixEngine/Render/RenderQueue.hpp>

static helix::Ref queue = new helix_cmd::CommandQueue<std::u8string>;

SDL_Window* sdlWindow = nullptr;

[[noreturn]] static void func_output(const std::stop_token& token)
{
	SDL_GLContext context = SDL_GL_CreateContext(sdlWindow);
	SDL_GL_MakeCurrent(sdlWindow, context);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
	while (!token.stop_requested())
	{
		static size_t num = 0;
		auto list = queue->receive();
		num++;
		for (const auto& item: list->getCommands())
		{
			if (*item == u8"clear")
			{
				glClearColor(0.f, 0.f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glColor3f(1, 0, 0);
				glRectf(-0.2f, -0.2f, 0.2f, 0.2f);
				continue;
			}
			if (*item == u8"swap")
			{
				SDL_GL_SwapWindow(sdlWindow);
			}
		}
	}
}

int main()
{
	helix::Ref window = new helix_sdl3::Window{u8"Hello, HelixEngine", {800, 600}};

	sdlWindow = window->getSDLWindow();

	std::jthread outputThread(func_output);

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
		}

		queue->addCommand<std::u8string>(u8"clear");
		queue->addCommand<std::u8string>(u8"swap");
		queue->commit();
	}
}
