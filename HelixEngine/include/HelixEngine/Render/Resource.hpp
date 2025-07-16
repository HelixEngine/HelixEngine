#pragma once
#include <optional>
#include <shared_mutex>
#include <HelixEngine/Math/Shape.hpp>
#include <HelixEngine/Math/Vector.hpp>
#include <HelixEngine/Util/Color.hpp>
#include <HelixEngine/Util/PixelFormat.hpp>
#include <HelixEngine/Util/Ref.hpp>
#include <sail-c++/sail-c++.h>

namespace helix
{
	class RenderResource : public Object
	{
	public:
		using Callback = std::function<void()>;
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
		void setNotifyCallback(Callback callback);
	private:
		std::atomic_bool bIsUsable = false;
		std::shared_mutex mtx;
		std::condition_variable_any cv;
		std::optional<Callback> callback;
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

		[[nodiscard]] static Ref<Bitmap> load(std::u8string_view filePath, const Config& config = {});
		[[nodiscard]] static Ref<Bitmap> loadAsync(std::u8string_view filePath, const Config& config = {});

		bool convert(const PixelFormat& dstFormat);

		[[nodiscard]] const PixelFormat& getPixelFormat() const;
		[[nodiscard]] Vector2UI32 getSize() const;
		[[nodiscard]] const sail::image& getSailImage() const;
	private:
		void innerLoad(std::u8string_view filePath, const Config& config);
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

		struct BitmapConfig
		{
			Ref<Bitmap> bitmap;
			PixelFormat textureFormat = PixelFormat::Unknown;
			Type textureType = Type::Default;
			bool isGenerateMipmap = false;
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
			None,
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
			//指定ClampBorder的颜色，当环绕方式为ClampBorder时生效
			Color borderColor = Color::White;

			Filter magFilter = Filter::Linear;
			Filter minFilter = Filter::Linear;
			Filter mipmapFilter = Filter::None;

			//当该项为std::nullopt时，表示不开启各向异性过滤
			//当该项为0时，表示默认使用各向异性过滤的系统最大值
			//当该项为非0时，表示使用指定的各向异性过滤值
			std::optional<uint32_t> maxAnisotropy = std::nullopt;
		};

		[[nodiscard]] const Config& getConfig() const;
	protected:
		Config config;

		Sampler() = default;
	};

	class Image final : public RenderResource
	{
		friend class Renderer;
	public:
		using Cache = std::unordered_map<std::u8string, Ref<Image>>;
		explicit Image(Ref<Texture2D> texture);

		[[nodiscard]] const Ref<Texture2D>& getTexture() const;
	private:
		Image() = default;
		Ref<Texture2D> texture;
	};
}
