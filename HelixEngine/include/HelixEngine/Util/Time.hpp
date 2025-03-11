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
		template<typename Period>
		using StdDuration = std::chrono::duration<float, Period>;
		using StdNanoseconds = StdDuration<std::nano>;
		using StdMilliseconds = StdDuration<std::milli>;
		using StdSeconds = StdDuration<std::chrono::seconds::period>;
		using StdMinutes = StdDuration<std::chrono::milliseconds::period>;
		using StdHours = StdDuration<std::chrono::hours::period>;
		using DataDurationType = StdSeconds;
	public:
		Duration();

		template<class Rep, class Period>
		Duration(const std::chrono::duration<Rep, Period> duration) :
			duration(std::chrono::duration_cast<DataDurationType>(duration))
		{
		}

		[[nodiscard]] float getHours() const;


		[[nodiscard]] float getMinutes() const;

		[[nodiscard]] float getSeconds() const;

		[[nodiscard]] float getMilliseconds() const;

		[[nodiscard]] float getNanoseconds() const;

		operator DataDurationType() const;

		template<class Rep, class Period>
		Duration& operator=(const std::chrono::duration<Rep, Period> duration)
		{
			this->duration = duration;
			return *this;
		}

		Duration operator+(Duration duration) const;

		Duration operator-(Duration duration) const;

		Duration& operator+=(Duration duration);

		Duration& operator-=(Duration duration);

		auto operator<=>(Duration duration) const
		{
			return this->duration <=> duration.duration;
		}
	private:
		DataDurationType duration{};
	};

	template<typename ClockType>
	class TimePoint final
	{
		using StdTimePoint = typename ClockType::StdClockType::time_point;

		StdTimePoint timePoint{};
	public:
		TimePoint() = default;

		TimePoint(StdTimePoint stdTimePoint) :
			timePoint(std::move(stdTimePoint))
		{
		}

		[[nodiscard]] Duration operator-(TimePoint timePoint) const
		{
			return this->timePoint - timePoint.timePoint;
		}

		[[nodiscard]] TimePoint operator-(Duration duration) const
		{
			return timePoint - duration;
		}

		[[nodiscard]] TimePoint& operator-=(Duration duration)
		{
			return timePoint -= duration;
		}

		[[nodiscard]] TimePoint operator+(Duration duration) const
		{
			return timePoint + duration;
		}

		[[nodiscard]] TimePoint& operator+=(Duration duration)
		{
			return timePoint += duration;
		}
	};

	template<typename StdClockTypeParam>
	class Clock final
	{
		template<typename ClockType>
		friend class TimePoint;
		using StdClockType = StdClockTypeParam;
	public:
		using TimePoint = TimePoint<Clock>;

		static TimePoint now()
		{
			return StdClockType::now();
		}
	};

	using SteadyClock = Clock<std::chrono::steady_clock>;
}
