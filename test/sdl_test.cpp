#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref renderer = window->getRenderer();

	Ref node = new Node2D;
	Ref child1 = new Node2D;
	child1->setName(u8"child1");
	Ref child2 = new Node2D;
	child2->setName(u8"child2");
	node->addChild(child1);
	node->addChild(child2);

	for (const auto& child: node->getAllChildren())
	{
		Logger::info(u8"child name:", child->getName());
	}

	auto n1 = node->findChild(u8"child2");

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
