#pragma once
#include <cstdint>

namespace helix
{
	struct RenderCommand
	{
		enum class Type : uint32_t
		{
			Begin,
			End,
		};

		Type type{};

		template<typename ActualType>
		ActualType* cast()
		{
			return reinterpret_cast<ActualType*>(this);
		}
	};
}
