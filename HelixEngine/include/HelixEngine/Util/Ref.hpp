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
		explicit(false) Ref(const Ref<RhsPointType>& rhs) :
			object(rhs.get())
		{
			increase();
		}

		explicit(false) Ref(PointType* object) :
			object(object)
		{
			increase();
		}

		explicit(false) Ref(nullptr_t)
		{
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

		// bool operator!=(const Ref& ref) const
		// 	noexcept
		// {
		// 	return ref.object != object;
		// }

		// bool operator!=(const PointType* object) const
		// 	noexcept
		// {
		// 	return this->object != object;
		// }

		explicit(false) operator bool() const
		{
			return object;
		}

		explicit(false) operator PointType*() const
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

	template<typename PointType>
	bool operator==(const Ref<PointType>& ref1, const Ref<PointType>& ref2)
		noexcept
	{
		return ref1.get() == ref2.get();
	}

	template<typename PointType>
	bool operator==(PointType* ref1, const Ref<PointType>& ref2)
		noexcept
	{
		return ref1 == ref2.get();
	}

	template<typename PointType>
	bool operator==(const Ref<PointType>& ref1, PointType* ref2)
		noexcept
	{
		return ref1.get() == ref2;
	}
}
