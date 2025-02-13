#pragma once
#include <HelixEngine/Base/Object.hpp>

namespace helix
{
	class Object;
	template<typename Type>
	concept BasedObject = std::is_base_of_v<Object, Type>;

	template<BasedObject TypeBasedObject>
	class Ref
	{
	public:
		Ref() = default;
	private:
		TypeBasedObject* object = nullptr;
	};
}
