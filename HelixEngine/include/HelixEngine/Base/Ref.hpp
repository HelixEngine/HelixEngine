#pragma once
#include <HelixEngine/Base/Object.hpp>

namespace helix
{
	template<BasedObject TypeBasedObject>
	class Ref
	{
	public:
		Ref() = default;

		Ref(Ref&& rhs) noexcept
		{
			rhs.swap(*this);
		}

		Ref(const Ref& rhs) :
			object(rhs.object)
		{
			increase();
		}

		// ReSharper disable once CppNonExplicitConvertingConstructor

		Ref(TypeBasedObject* object) : // NOLINT(*-explicit-constructor)
			object(object)
		{
			increase();
		}

		~Ref()
		{
			decrease();
			destroy();
		}

		Ref& operator=(const Ref& ref)
			noexcept
		{
			Ref(ref).swap(*this);
			return *this;
		}

		Ref& operator=(Ref&& ref)
			noexcept
		{
			ref.swap(*this);
			return *this;
		}

		void swap(Ref& ref)
			noexcept
		{
			std::swap(object, ref.object);
		}

		void reset(TypeBasedObject* ptr = nullptr)
		{
			Ref(ptr).swap(*this);
		}

		[[nodiscard]] TypeBasedObject* get() const
			noexcept
		{
			return object;
		}

		TypeBasedObject* operator->() const
			noexcept
		{
			return object;
		}

		TypeBasedObject& operator*() const
		{
			return *object;
		}

		bool operator==(const Ref& ref) const
			noexcept
		{
			return ref.object == object;
		}

		bool operator==(const TypeBasedObject* ref) const
			noexcept
		{
			return ref == object;
		}

		bool operator!=(const Ref& ref) const
			noexcept
		{
			return ref.object != object;
		}

		bool operator!=(const TypeBasedObject* object) const
			noexcept
		{
			return this->object != object;
		}
	private:
		void increase()
		{
			if (object)
				++object->refCount;
		}

		void decrease()
		{
			if (object)
				--object->refCount;
		}

		void destroy()
		{
			if (object && object->refCount == 0)
				delete object;
		}

		TypeBasedObject* object = nullptr;
	};
}
