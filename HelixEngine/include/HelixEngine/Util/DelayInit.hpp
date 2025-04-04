#pragma once
#include <functional>
#include <optional>

namespace helix
{
	template<typename T>
	class DelayInit
	{
		mutable std::optional<T> instance;
		std::function<T()> initializer;
	public:
		explicit DelayInit(std::function<T()> initializer = [] { return T{}; }) :
			initializer(std::move(initializer))
		{
		}

		DelayInit(DelayInit&&) = delete;
		DelayInit(const DelayInit&) = delete;

		DelayInit& operator=(DelayInit&&) = delete;
		DelayInit& operator=(const DelayInit&) = delete;

		[[nodiscard]] T& get()
		{
			if (instance.has_value())
				return instance.value();
			instance = std::move(initializer());
			return instance.value();
		}

		[[nodiscard]] const T& get() const
		{
			if (instance.has_value())
				return instance.value();
			instance = std::move(initializer());
			return instance.value();
		}

		[[nodiscard]] bool isInit() const noexcept
		{
			return instance.has_value();
		}
	};
}
