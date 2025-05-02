#pragma once
#include <HelixEngine/Util/Object.hpp>

namespace helix
{
	class VertexBuffer : public Object
	{
	public:
		enum class Usage
		{
			Static,
			Dynamic,
			Stream,
		};
	private:
		Usage usage = Usage::Static;
	public:
		[[nodiscard]] Usage getUsage() const
		{
			return usage;
		}
	protected:
		void setUsage(Usage usage)
		{
			this->usage = usage;
		}
	};
}
