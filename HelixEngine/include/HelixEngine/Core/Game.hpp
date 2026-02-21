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
		friend class Renderer;
		template<typename CommandType>
		friend class CommandQueue;
	public:
		enum class State
		{
			Setup,
			Start,
			Run,
			Quit,
		};
		using StartCallback = std::function<void()>;
		static void init();
		static int run();
		static void setStartCallback(StartCallback callback);
		static State getState();
	private:
		static inline std::optional<StartCallback> startCallback;
		static inline State state = State::Setup;
	};

}
