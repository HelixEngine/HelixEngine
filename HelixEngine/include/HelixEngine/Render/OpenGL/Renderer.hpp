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

		//resource method//

		[[nodiscard]] Ref<helix::MemoryBuffer> createNativeMemoryBuffer(
				MemoryBuffer::Type type,
				MemoryBuffer::Usage usage, Ref<MemoryBlock> vertexData) const override;

		[[nodiscard]] Ref<opengl::Shader> createNativeShader(Shader::Usage usage);

		[[nodiscard]] static Ref<opengl::RenderPipeline> createNativeRenderPipeline();

		//render thread//

		void startRun() override;

		//cmd process//

		RenderCommand* renderCmd{};

		Ref<SDLOpenGLContext> sdlContext = new SDLOpenGLContext;

		//Command Status
		PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;
		IndexAttribute indexAttribute;

		[[nodiscard]] SDL_GLContext createSDLContext() const;
		void makeCurrentContext(SDL_GLContext context) const;
		[[nodiscard]] SDL_GLContext createCurrentSDLContext() const;

		void renderProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;
		void setRenderPipelineProc() const;
		void setPrimitiveTopologyProc();
		void setViewportProc() const;
		void setScissorProc() const;
		void drawProc() const;
		void drawIndexedProc() const;
		void setGLVertexArrayProc();
		void setGLUniformBindingAttributeProc() const;

		void createGLVertexArrayProc() const;

		void createMemoryBufferProc() const;
		void createGLShaderProc() const;
		void createGLRenderPipelineProc() const;
		void destroyGLShaderProc() const;

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
	private:
		void destroyGLShader(const Shader* shader) const;

		void readyRender() override;

		void renderThreadFunc(const std::stop_token& token) override;

		static void gladDebugOutput(const char* name, void* funcPtr, int lenArgs, ...);
		static inline bool isInitDebugOutput = false;
	};
}
