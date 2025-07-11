#pragma once
#include <array>

namespace helix
{
	enum class MessageLevel
	{
		Info,
		Warning,
		Error,
	};

	enum class GraphicsApi
	{
		OpenGL,
		Default = OpenGL,
	};

	enum class PrimitiveTopology
	{
		TriangleList,
	};
}
