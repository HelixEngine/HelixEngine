#pragma once
#include <HelixEngine/Util/Object.hpp>

namespace helix
{

	template<typename PointType>
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

		template<typename RhsPointType> requires BasedObject<RhsPointType, PointType>
		Ref(const Ref<RhsPointType>& rhs) :
			object(rhs.get())
		{
			increase();
		}

		// ReSharper disable once CppNonExplicitConvertingConstructor

		Ref(PointType* object) requires BasedObject<PointType>: // NOLINT(*-explicit-constructor)
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

		void reset(PointType* ptr = nullptr)
		{
			Ref(ptr).swap(*this);
		}

		[[nodiscard]] PointType* get() const
			noexcept
		{
			return object;
		}

		PointType* operator->() const
			noexcept
		{
			return object;
		}

		PointType& operator*() const
		{
			return *object;
		}

		bool operator==(const Ref& ref) const
			noexcept
		{
			return ref.object == object;
		}

		bool operator==(const PointType* ref) const
			noexcept
		{
			return ref == object;
		}

		bool operator!=(const Ref& ref) const
			noexcept
		{
			return ref.object != object;
		}

		bool operator!=(const PointType* object) const
			noexcept
		{
			return this->object != object;
		}

		operator bool() const
		{
			return object;
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

		PointType* object = nullptr;
	};
}
