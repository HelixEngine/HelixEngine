#pragma once
#include <HelixEngine/Util/Ref.hpp>

#include "HelixEngine/Math/Shape.hpp"
#include "HelixEngine/Math/Vector2.hpp"

namespace helix
{
	class VertexBuffer : public Object
	{
	public:
		enum class Usage
		{
			Static,
			Dynamic,
			Stream,
		};
	private:
		Usage usage = Usage::Static;
	public:
		[[nodiscard]] Usage getUsage() const
		{
			return usage;
		}
	protected:
		void setUsage(Usage usage)
		{
			this->usage = usage;
		}
	};

	class Pipeline : public Object
	{

	};

	class Shader : public Object
	{
	public:
		enum class Usage
		{
			Vertex,
			Pixel,
		};
	private:
		Usage usage = Usage::Vertex;
	public:
		[[nodiscard]] Usage getUsage() const
		{
			return usage;
		}
	protected:
		void setUsage(Usage usage)
		{
			this->usage = usage;
		}
	};

	class RenderPipeline : public Pipeline
	{
	public:
		struct Config final
		{
			Ref<Shader> vertex{};
			Ref<Shader> pixel{};
		};
	};

	struct Viewport final
	{
		Rect<Vector2F, Vector2F> area;
		float minDepth = 0;
		float maxDepth = 0;
	};

	using Scissor = Rect<Vector2I32, Vector2I32>;
}
