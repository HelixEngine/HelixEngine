#pragma once
#include <concepts>
#include <bit>

namespace helix
{
	template<typename ItemType>
	concept BitEnum = std::is_enum_v<ItemType> &&
	                  std::unsigned_integral<std::underlying_type_t<ItemType>>;

	template<BitEnum ItemType>
	class BitOption
	{
	public:
		using ValueType = std::underlying_type_t<ItemType>;
	private:
		ValueType value{};
	public:
		BitOption() = default;

		explicit(false) BitOption(ItemType item)
		{
			value = static_cast<ValueType>(item);
		}

		void setItem(ItemType item, bool isEnabled = true)
		{
			if (isEnabled)
				value |= static_cast<ValueType>(item);
			else
				value &= ~static_cast<ValueType>(item);
		}

		[[nodiscard]] bool getItem(ItemType item) const
		{
			return static_cast<ValueType>(item) & value;
		}

		[[nodiscard]] bool isContain(BitOption option) const
		{
			return (option.value & value) == option.value;
		}

		[[nodiscard]] uint32_t getEnabledItemSize() const
		{
			return std::popcount(value);
		}
	};
}
