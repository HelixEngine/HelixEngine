#pragma once

namespace helix
{
	template<typename ItemType>
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
	};
}
