#pragma once
#include <forward_list>
#include <memory>
#include <deque>
#include <mutex>
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace helix
{
	class RenderCommandBuffer final : public Object
	{
		friend class RenderQueue;
		friend class Renderer;
		std::deque<std::unique_ptr<RenderCommand>> commands;

		template<typename CommandType, typename... Args>
		void addCommand(Args&&... args)
		{
			commands.push_back(std::make_unique<CommandType>(std::forward<Args>(args)...));
		}
	public:
		//取出命令，最后一个命令必定为End
		std::unique_ptr<RenderCommand> acquireCommand();
		[[nodiscard]] bool empty() const;
	};

	class RenderQueue final : public Object
	{
		friend class Renderer;
		friend class RenderInstance;
		//大致流程：Update端输入命令->Update端交换缓冲->Render端取出命令->Render端执行命令
		//这里的Update端和Render端是指两个线程
		//若Update输入命令完成，但Render端还未执行完命令，则Update端会将命令存放在tempBuffer中，开始新的Update
		//若Render端执行完命令，但Update端还在输入命令，如果tempBuffer已经刷新，则与tempBuffer交换，然后开始新的Render，否则继续等待，直到tempBuffer或frontBuffer刷新

		//给Update端输入命令的buffer
		Ref<RenderCommandBuffer> frontBuffer = new RenderCommandBuffer;
		//用于暂存命令的buffer
		Ref<RenderCommandBuffer> tempBuffer = new RenderCommandBuffer;
		//给Render端存放命令的buffer
		//要求RenderThread为backBuffer提供一个有效的buffer
		Ref<RenderCommandBuffer> backBuffer;

		//同步原语
		std::mutex mtx;
		bool isFrontBufferReady = false;

		void swapFrontBuffer();
		void swapBackBuffer();
	public:
		template<typename CommandType, typename... Args>
		void addCommand(Args&&... args)
		{
			frontBuffer->addCommand<CommandType>(std::forward<Args>(args)...);
		}

		void setBackBuffer(Ref<RenderCommandBuffer> buffer);
		//获取当前的backBuffer
		//如果返回nullptr，第一种情况，RenderThread还未提供backBuffer，第二种情况，客户端要求RenderThread退出
		[[nodiscard]] const Ref<RenderCommandBuffer>& getBackBuffer() const;
		[[nodiscard]] const Ref<RenderCommandBuffer>& getFrontBuffer() const;
	};
}
