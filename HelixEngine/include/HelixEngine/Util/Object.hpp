#pragma once
#include <atomic>
#include <cstdint>
#include <string>

namespace helix
{
	class Object;
	template<typename Type, typename Base = Object>
	concept BasedObject = std::is_base_of_v<Base, Type>;

	class Object
	{
		template<typename PointType>
		friend class Ref;
	public:
		virtual ~Object() = default;
		void setName(std::u8string name);
		[[nodiscard]] const std::u8string& getName() const;
		[[nodiscard]] uint64_t getRefCount() const;
	private:
		std::u8string name;
		std::atomic<uint64_t> refCount = 0;
	};
}
