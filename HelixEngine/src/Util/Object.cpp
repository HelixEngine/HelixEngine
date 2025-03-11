#include <HelixEngine/Util/Object.hpp>

void helix::Object::setName(std::u8string name)
{
	this->name = std::move(name);
}

const std::u8string& helix::Object::getName() const
{
	return name;
}

uint64_t helix::Object::getRefCount() const
{
	return refCount;
}
