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
	class Renderer final : public helix::Renderer
	{
		friend class Window;

		//GL初始化时，如果2个窗口同时初始化，会出现内存访问冲突导致崩溃
		static inline std::mutex glInitMtx;

		//resource method//

		[[nodiscard]] Ref<helix::VertexBuffer> createNativeVertexBuffer(
				VertexBuffer::Usage usage,
				Ref<MemoryBlock> vertexData) const override;

		[[nodiscard]] static Ref<opengl::Shader> createNativeShader(
				Shader::Usage usage);

		//render thread//

		void startRun() override;

		void renderLoopFunc();
		std::stop_token renderToken;

		//cmd process//

		RenderCommand* renderCmd{};
		ResourceCommand* resourceCmd{};
		SDL_GLContext renderContext{};
		SDL_GLContext resourceContext{};
		[[nodiscard]] SDL_GLContext createSDLContext() const;
		void makeCurrentContext(SDL_GLContext context) const;
		[[nodiscard]] SDL_GLContext createCurrentSDLContext() const;

		void renderProc(const RenderQueue::ListRef& list);
		void beginProc() const;
		void endProc() const;

		void resourceProc(const ResourcePipeline::ListRef& list);
		void createVertexBufferProc() const;
		void createGLShaderProc() const;
	public:
		[[nodiscard]] Ref<opengl::Shader> createGLShader(Shader::Usage usage, std::u8string shaderCode) const;
	};
}
