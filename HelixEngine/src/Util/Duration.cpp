#include <HelixEngine/Util/Time.hpp>

helix::Duration::Duration() = default;

float helix::Duration::hours() const
{
	return minutes() / 60.f;
}

float helix::Duration::minutes() const
{
	return seconds() / 60.f;
}

float helix::Duration::seconds() const
{
	return static_cast<float>(milliseconds()) / 1000.f;
}

std::uint64_t helix::Duration::milliseconds() const
{
	return ms.count();
}

helix::Duration::operator std::chrono::duration<long long, std::ratio<1, 1000>>() const
{
	return ms;
}

auto helix::Duration::operator+(const Duration duration) const
{
	return Duration{ms + duration.ms};
}

auto helix::Duration::operator-(const Duration duration) const
{
	return Duration{ms - duration.ms};
}

helix::Duration& helix::Duration::operator+=(const Duration duration)
{
	ms += duration.ms;
	return *this;
}

helix::Duration& helix::Duration::operator-=(const Duration duration)
{
	ms -= duration.ms;
	return *this;
}

auto helix::Duration::operator<=>(const Duration duration) const
{
	return ms <=> duration.ms;
}
