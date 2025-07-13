#pragma once
#include <optional>
#include <shared_mutex>
#include <HelixEngine/Util/Ref.hpp>
#include <HelixEngine/Util/PixelFormat.hpp>
#include <HelixEngine/Math/Shape.hpp>
#include <HelixEngine/Math/Vector.hpp>
#include <sail-c++/sail-c++.h>

namespace helix
{
	class RenderResource : public Object
	{
	public:
		/**
		 * @brief 等待资源可用
		 * @note 对于共享资源，在渲染端使用资源时，也需要调用该方法确保资源可用
		 */
		void usable();

		/**
		 * @brief 通知资源可用
		 */
		void notify();

		[[nodiscard]] bool isUsable() const;
	private:
		std::atomic_bool bIsUsable = false;
		std::shared_mutex mtx;
		std::condition_variable_any cv;
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
		[[nodiscard]] Type getType() const;

		[[nodiscard]] Usage getUsage() const;
	protected:
		void setType(Type type);

		void setUsage(Usage usage);

		MemoryBuffer() = default;
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
		void setUsage(Usage usage);
		Shader() = default;
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

	class Bitmap : public RenderResource
	{
		friend class Renderer;
	public:
		struct Config
		{
			PixelFormat bitmapFormat = PixelFormat::Unknown;
		};

		[[nodiscard]] static Ref<Bitmap> load(const std::u8string& filePath, const Config& config = {});

		bool convert(const PixelFormat& dstFormat);

		[[nodiscard]] const PixelFormat& getPixelFormat() const;
		[[nodiscard]] Vector2UI32 getSize() const;
		[[nodiscard]] const sail::image& getSailImage() const;
	private:
		void innerLoad(const std::u8string& filePath, const Config& config);
		Bitmap() = default;
		sail::image image;
		PixelFormat format;

		template<typename SrcFormat, typename DstFormat>
		struct ConvertInfo
		{
			SrcFormat srcFormat{};
			bool isNeedConvert = false;
			DstFormat dstFormat{};
		};

		static SailPixelFormat formatConvert(const PixelFormat& format);
		static ConvertInfo<helix::PixelFormat, SailPixelFormat> formatConvert(SailPixelFormat format);

		static bool imageConvertFormat(sail::image& image, SailPixelFormat dstFormat);
	};

	class Texture2D : public RenderResource
	{
	public:
		enum class Type
		{
			Default,
			Storage,
		};

		[[nodiscard]] const PixelFormat& getPixelFormat() const;

		[[nodiscard]] Vector2UI32 getSize() const;

		[[nodiscard]] Type getType() const;
	private:
		PixelFormat format;
		Vector2UI32 size;
		Type type = Type::Default;
	protected:
		void setPixelFormat(const PixelFormat& format);

		void setSize(Vector2UI32 size);

		void setType(Type type);

		Texture2D() = default;
	};

	class Sampler : public RenderResource
	{
	public:
		enum class Warp
		{
			Repeat,
			Mirror,
			ClampEdge,
			ClampBorder,
		};

		enum class Filter
		{
			Linear,
			Nearest,
		};

		struct Config final
		{
			//纹理采样的环绕方式
			//x -> s
			//y -> t
			//z -> r
			//各分量根据实际的采样纹理生效
			Vector3E<Warp> warp;
			Filter minFilter = Filter::Linear;
			Filter magFilter = Filter::Linear;
			//当所采样的纹理开启了Mipmap后，该项生效
			Filter mipmapFilter = Filter::Linear;
			//当该项为std::nullopt时，表示不开启各向异性过滤
			//当该项为0时，表示默认使用各向异性过滤的系统最大值
			//当该项为非0时，表示使用指定的各向异性过滤值
			std::optional<uint32_t> maxAnisotropy = std::nullopt;
		};

		[[nodiscard]] const Config& getConfig() const;
	private:
		Config config;
	protected:
		Sampler() = default;
	};
}
