#pragma once

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
}
