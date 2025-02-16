#pragma once
#include <QApplication>

namespace helix
{
	class Game final
	{
	public:
		static void init(int argc, char** argv);
		static int run();
	private:
		static inline std::unique_ptr<QApplication> qApplication;
	};
}
