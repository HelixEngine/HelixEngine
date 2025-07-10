#pragma once
#include <memory>
#include <deque>
#include <mutex>
#include <optional>
#include <HelixEngine/Util/Ref.hpp>

#include "HelixEngine/Core/Game.hpp"
#include "HelixEngine/Util/Logger.hpp"

namespace helix
{

	template<typename CommandType>
	class CommandQueue;

	/**
	 * @brief 命令列表
	 * @note 用于存储一系列给定的命令
	 * @tparam CommandType 命令类型
	 */
	template<typename CommandType>
	class CommandList final : public Object
	{
		friend class CommandQueue<CommandType>;
		std::deque<std::unique_ptr<CommandType>> commands;
	public:
		template<typename ActualType, typename... Args>
		void addCommand(Args&&... args)
		{
			commands.push_back(std::make_unique<ActualType>(std::forward<Args>(args)...));
		}

		void clear()
		{
			commands.clear();
		}

		const std::deque<std::unique_ptr<CommandType>>& getCommands() const
		{
			return commands;
		}
	};

	/**
	 * @brief 命令队列
	 * @note 用于在两个线程之间传递命令，区分发送端和接收端
	 * @tparam CommandType 命令类型
	 */
	template<typename CommandType>
	class CommandQueue final : public Object
	{
	public:
		using ListType = CommandList<CommandType>;
		using ListRef = Ref<ListType>;
	private:
		alignas(std::hardware_destructive_interference_size) ListRef front = new CommandList<CommandType>;
		alignas(std::hardware_destructive_interference_size) ListRef back = new CommandList<CommandType>;

		std::atomic_bool isCommited = false;
	public:
		template<typename ActualType, typename... Args>
		void addCommand(Args&&... args)
		{
			front->template addCommand<ActualType>(std::forward<Args>(args)...);
		}

		/**
		 * @brief 提交命令列表
		 * @note 提交后，接收端将会收到一个新的命令列表，
		 * 而发送端将会获得一个空的命令列表
		 */
		void commit()
		{
			isCommited.store(true, std::memory_order_seq_cst);
			while (isCommited.load(std::memory_order_seq_cst))
				std::this_thread::yield();
		}

		/**
		 * @brief 接收一个 CommandList
		 * @return	1.如果当前有新命令，则返回一个新的CommandList\n
		 *			2.如果当前没有新命令，则返回先前的CommandList\n
		 */
		[[nodiscard]] ListRef receive()
		{
			if (isCommited.load(std::memory_order_acquire))
			{
				back->clear();
				back.swap(front);
				isCommited.store(false, std::memory_order_release);
			}
			return back;
		}
	};
}
