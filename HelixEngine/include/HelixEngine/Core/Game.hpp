#pragma once
#include <HelixEngine/Util/Singleton.hpp>

namespace helix
{
	// class Game final
	// {
	// 	friend class Window;
	// public:
	// 	static void setCommandLineArguments(int argc, char** argv);
	// 	static int run();
	// private:
	// 	struct Instance final : Singleton<Instance>
	// 	{
	// 		Instance();
	// 		int argc = 0;
	// 		char** argv = nullptr;
	// 	};
	// };

	class Game final
	{
	public:
		static int run();
	};

}
