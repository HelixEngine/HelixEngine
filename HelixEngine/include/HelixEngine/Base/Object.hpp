#pragma once
#include <cstdint>
#include <string>

namespace helix
{
	class Object
	{
	public:
		void setName(std::u8string name);
		[[nodiscard]] const std::u8string& getName() const;
		[[nodiscard]] uint64_t getRefCount() const;
	private:
		std::u8string name;
		uint64_t refCount = 0;
	};
}
