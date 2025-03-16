#pragma once

namespace helix
{
	template<typename ItemType>
	class Feature
	{
	public:
		using ValueType = std::underlying_type_t<ItemType>;
	private:
		ValueType value{};
	public:
		Feature() = default;

		explicit(false) Feature(ItemType item)
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
