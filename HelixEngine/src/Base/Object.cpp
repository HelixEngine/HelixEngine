#include <HelixEngine/Base/Object.hpp>

void helix::Object::setName(std::u8string name)
{
	this->name = std::move(name);
}

const std::u8string& helix::Object::getName() const
{
	return name;
}
