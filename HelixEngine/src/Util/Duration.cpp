#include <HelixEngine/Util/Time.hpp>

helix::Duration::Duration() = default;

float helix::Duration::hours() const
{
	return std::chrono::duration_cast<StdHours>(duration).count();
}

float helix::Duration::minutes() const
{
	return std::chrono::duration_cast<StdSeconds>(duration).count();
}

float helix::Duration::seconds() const
{
	return std::chrono::duration_cast<StdSeconds>(duration).count();
}

float helix::Duration::milliseconds() const
{
	return std::chrono::duration_cast<StdMilliseconds>(duration).count();
}

float helix::Duration::nanoseconds() const
{
	return std::chrono::duration_cast<StdNanoseconds>(duration).count();
}

helix::Duration::operator helix::Duration::DataDurationType() const
{
	return duration;
}

helix::Duration helix::Duration::operator+(const Duration duration) const
{
	return Duration{this->duration + duration.duration};
}

helix::Duration helix::Duration::operator-(const Duration duration) const
{
	return Duration{this->duration - duration.duration};
}

helix::Duration& helix::Duration::operator+=(const Duration duration)
{
	this->duration += duration.duration;
	return *this;
}

helix::Duration& helix::Duration::operator-=(const Duration duration)
{
	this->duration -= duration.duration;
	return *this;
}
