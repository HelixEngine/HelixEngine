#pragma once
#include <Essence/Queue.hpp>
#include <vulkan/vulkan.h>

namespace essence::vulkan
{
	class Queue final : public essence::Queue
	{
		VkQueue queue = nullptr;
	public:
		VkQueue getVkQueue() const;
	};
}
