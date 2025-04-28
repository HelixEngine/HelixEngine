#pragma once
#include <forward_list>
#include <memory>
#include <deque>
#include <mutex>
#include <optional>
#include <HelixEngine/Render/Command/RenderCommand.hpp>
#include <HelixEngine/Util/Ref.hpp>

namespace helix
{

	template<typename CommandType>
	class CommandQueue;

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
		 *			3.如果程序要求退出，则返回std::nullopt
		 */
		[[nodiscard]] std::optional<ListRef> receive()
		{
			std::lock_guard lock(mtx);
			if (isCommited)
			{
				back.swap(staging);
				isCommited = false;
			}

			return back->getCommands().empty() ? std::nullopt : std::optional<ListRef>{back};
		}

		void quit()
		{
			std::lock_guard lock(mtx);
			staging->clear();
			isCommited = true;
		}
	};
}
