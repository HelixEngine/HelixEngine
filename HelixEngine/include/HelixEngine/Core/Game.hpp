#pragma once
#include <QApplication>
#include <HelixEngine/Base/Singleton.hpp>

namespace helix
{
	class Game final
	{
		friend class Window;
	public:
		static void setCommandLineArguments(int argc, char** argv);
		static int run();
	private:
		struct Instance final : Singleton<Instance>
		{
			Instance();
			std::unique_ptr<QApplication> qApplication{};
			int argc = 0;
			char** argv = nullptr;
		};
	};
}
