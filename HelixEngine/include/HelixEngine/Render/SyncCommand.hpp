#pragma once
#include <memory>
#include <deque>
#include <mutex>
#include <optional>
#include <HelixEngine/Util/Ref.hpp>

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
	 * @note 用于在两个线程之间传递命令，区分发送端和接收端\n
	 * 两端之间完全异步，接收端只处理最新的命令，旧的命令（哪怕未处理）将被丢弃
	 * @tparam CommandType 命令类型
	 */
	template<typename CommandType>
	class CommandQueue final : public Object
	{
	public:
		using ListType = CommandList<CommandType>;
		using ListRef = Ref<ListType>;
	private:
		ListRef front = new CommandList<CommandType>;
		ListRef staging = new CommandList<CommandType>;
		ListRef back = new CommandList<CommandType>;

		//同步原语
		std::mutex mtx;
		bool isCommited = false;
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
			std::lock_guard lock(mtx);
			front.swap(staging);
			front->clear();
			isCommited = true;
		}

		/**
		 * @brief 接收一个 CommandList
		 * @return	1.如果当前有新命令，则返回一个新的CommandList\n
		 *			2.如果当前没有新命令，则返回先前的CommandList\n
		 */
		[[nodiscard]] ListRef receive()
		{
			std::lock_guard lock(mtx);
			if (isCommited)
			{
				back.swap(staging);
				isCommited = false;
			}

			return back;
		}
	};

	template<typename CommandType>
	class CommandPipeline final : public Object
	{
	public:
		using ListType = CommandList<CommandType>;
		using ListRef = Ref<ListType>;
	private:
		ListRef front = new CommandList<CommandType>;
		ListRef back = new CommandList<CommandType>;

		//同步原语
		std::mutex mtx;
		std::condition_variable cv;
		bool hasNewCommand = false;
	public:
		~CommandPipeline() override
		{
			quit();
		}

		template<typename ActualType, typename... Args>
		void addCommand(Args&&... args)
		{
			std::lock_guard lock(mtx);
			front->template addCommand<ActualType>(std::forward<Args>(args)...);
			hasNewCommand = true;
			cv.notify_all();
		}

		ListRef receive()
		{
			std::unique_lock lock(mtx);
			cv.wait(lock, [this] { return hasNewCommand; });
			back.swap(front);
			front->clear();
			hasNewCommand = false;
			return back;
		}

		void quit()
		{
			std::lock_guard lock(mtx);
			hasNewCommand = true;
			back->clear();
			cv.notify_all();
		}
	};

}
