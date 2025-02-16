#pragma once

namespace helix
{
	template<typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		~Singleton() = default;
	public:
		Singleton(Singleton&&) = delete;
		Singleton(const Singleton&) = delete;

		Singleton& operator=(Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		static T& getInstance()
		{
			static T instance;
			return instance;
		}
	};
}
