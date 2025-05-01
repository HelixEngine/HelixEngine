#include <HelixEngine/HelixEngine.hpp>
#include <HelixEngine/Render/OpenGL/Renderer.hpp>

using namespace helix;

int main()
{
	Ref window = new Window{u8"Hello, HelixEngine", {800, 600}};
	Ref scene = new Scene2D;
	window->enter(scene);

	//写一下SDL的main loop
	return Game::run();
}
