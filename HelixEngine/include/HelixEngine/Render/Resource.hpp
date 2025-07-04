#pragma once
#include <shared_mutex>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Math/Shape.hpp>
#include <HelixEngine/Math/Vector2.hpp>

namespace helix
{
	class RenderResource : public Object
	{
	public:
		/**
		 * @brief 等待资源可用
		 * @note 对于共享资源，在渲染端使用资源时，也需要调用该方法确保资源可用
		 */
		void usable()
		{
			std::shared_lock lock{mtx};
			if (isUsable)
				return;
			cv.wait(lock, [this]
			{
				return isUsable;
			});
		}

		/**
		 * @brief 通知资源可用
		 */
		void notify()
		{
			std::unique_lock lock{mtx};
			isUsable = true;
			cv.notify_all();
		}
	private:
		std::shared_mutex mtx;
		std::condition_variable_any cv;
		bool isUsable = false;
	};

	class MemoryBuffer : public RenderResource
	{
	public:
		enum class Type
		{
			Geometry,
			Uniform,
			Storage,
		};

		enum class Usage
		{
			Static,
			Dynamic,
			Stream,
		};
	private:
		Type type = Type::Geometry;
		Usage usage = Usage::Static;
	public:
		[[nodiscard]] Type getType() const
		{
			return type;
		}

		[[nodiscard]] Usage getUsage() const
		{
			return usage;
		}
	protected:
		void setType(Type type)
		{
			this->type = type;
		}

		void setUsage(Usage usage)
		{
			this->usage = usage;
		}
	};

	class Pipeline : public RenderResource
	{

	};

	class Shader : public RenderResource
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
