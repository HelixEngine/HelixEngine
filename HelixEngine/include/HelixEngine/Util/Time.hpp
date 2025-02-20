#pragma once
#include <chrono>

namespace helix
{
	namespace literals
	{
		using namespace std::chrono_literals;
	}

	using namespace literals;

	class Duration final
	{
	public:
		Duration();

		template<class Rep, class Period>
		Duration(const std::chrono::duration<Rep, Period> duration) :
			ms(duration)
		{
		}

		[[nodiscard]] float hours() const;


		[[nodiscard]] float minutes() const;

		[[nodiscard]] float seconds() const;

		[[nodiscard]] std::uint64_t milliseconds() const;

		operator std::chrono::milliseconds() const;

		template<class Rep, class Period>
		Duration& operator=(const std::chrono::duration<Rep, Period> ms)
		{
			this->ms = ms;
			return *this;
		}

		auto operator+(Duration duration) const;

		auto operator-(Duration duration) const;

		Duration& operator+=(Duration duration);

		Duration& operator-=(Duration duration);

		auto operator<=>(Duration duration) const;
	private:
		std::chrono::milliseconds ms;
	};
}
