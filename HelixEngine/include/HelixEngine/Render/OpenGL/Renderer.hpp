#pragma once
#include <HelixEngine/Core/Window.hpp>
#include <HelixEngine/Render/Renderer.hpp>
#include <HelixEngine/Render/OpenGL/Resource.hpp>

namespace helix
{
	class Window;
}

namespace helix::opengl
{
	struct SDLOpenGLContext : Object
	{
		SDL_GLContext context{};
		~SDLOpenGLContext() override;
	};

	class Renderer final : public helix::Renderer
	{
		friend class Window;
		friend class Shader;

		//GL初始化时，如果多个窗口同时初始化，会出现内存访问冲突导致崩溃
		static inline std::mutex glInitMtx;

		//native resource method//

		[[nodiscard]] Ref<helix::MemoryBuffer> createNativeMemoryBuffer(
				MemoryBuffer::Type type,
				MemoryBuffer::Usage usage, Ref<MemoryBlock> vertexData) const override;

		[[nodiscard]] Ref<helix::Texture2D>
		createNativeTexture2D(Texture2D::BitmapConfig config) const override;

		[[nodiscard]] Ref<helix::Sampler> createNativeSampler(const Sampler::Config& config) const override;

		[[nodiscard]] Ref<opengl::Shader> createNativeShader(Shader::Usage usage);

		Ref<helix::Shader> createNativeShader(Shader::Usage usage,
		                                      const EmbeddedShader::ShaderCodeCompiler& compiler) override;

		Ref<helix::RenderPipeline> createNativeRenderPipeline(helix::RenderPipeline::Config config) const override;

		EmbeddedShader::CompilerOption getCompilerOption() const override;

		//render thread//

		void startRun() override;

		//cmd process//

		RenderCommand* renderCmd{};

		Ref<SDLOpenGLContext> sdlContext = new SDLOpenGLContext;
		std::atomic_bool releaseContext = false;
		std::atomic_bool resumeContext = true;

		//Command Status
		PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;
		Ref<RenderPipeline> renderPipeline;
		IndexAttribute indexAttribute;

		void createSDLContext() const;
		void makeCurrentContext(SDL_GLContext context) const;

		void renderProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;
		void setRenderPipelineProc();
		void setPrimitiveTopologyProc();
		void setViewportProc() const;
		void setScissorProc() const;
		void drawProc() const;
		void drawIndexedProc() const;
		void setGLVertexArrayProc();
		void setGLUniformBindingAttributeProc() const;

		void createGLVertexArrayProc() const;

		void createMemoryBufferProc() const;
		void createTexture2DFromBitmapProc() const;
		void createSamplerProc() const;
		void createGLShaderProc() const;
		void createGLRenderPipelineProc() const;
		void destroyGLShaderProc() const;
		void setGLTexture2DUnitProc() const;

		//gl tool func
		static void attachGLShader(const RenderPipeline* pipeline, const helix::Shader* shader);
		static GLenum getGLPrimitiveTopology(PrimitiveTopology topology);
	public:
		[[nodiscard]] Ref<opengl::Shader> createGLShader(Shader::Usage usage, std::u8string shaderCode);
		[[nodiscard]] Ref<opengl::RenderPipeline> createGLRenderPipeline(RenderPipeline::Config config) const;

		[[nodiscard]] Ref<opengl::VertexArray> createGLVertexArray(VertexArray::Config config) const;
		void setGLVertexArray(Ref<VertexArray> vertexArray) const;
		void setGLUniformBindingAttribute(
				UniformBindingAttribute uniformBindingAttribute) const;
		void setGLTexture2DUnit(Texture2DUnit unit) const;
	private:
		void destroyGLShader(const Shader* shader) const;

		void renderThreadFunc(const std::stop_token& token) override;

		static void gladDebugOutput(const char* name, void* funcPtr, int lenArgs, ...);
		static inline bool isInitDebugOutput = false;
	};
}
