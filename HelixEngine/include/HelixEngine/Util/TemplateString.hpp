#pragma once
#include <algorithm>
namespace helix
{
	template<typename C,size_t N>
	struct TemplateString
	{
		constexpr explicit(false) TemplateString(const C(&arr)[N])
		{
			std::copy(arr,arr + N, string);
		}
		C string[N];
	};

}
