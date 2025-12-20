#pragma once

#include <atomic>
#include <type_traits>

// TO DO: Move buffer into this file as it is similarly a memory holder class
// add should probably also being included inside core

namespace daedalus {
#pragma region IntrusivePtr

	namespace intrusivePtrInternal {

		// foward declare
		struct IntrusiveCounterInternal;

		typedef uint64_t ThreadUnsafeCounterType;
		typedef std::atomic<uint64_t> ThreadSafeCounterType;

		template<typename T>
		struct IntrusiveCounter
		{
			friend struct IntrusiveCounterInternal;
		protected:
			IntrusiveCounter() = default;
		private:
			T count{ 0 };
		};

		struct IntrusiveCounterInternal
		{
			template<typename CT>
			static void increment(IntrusiveCounter<CT>* ptr);
			template<>
			static void increment(IntrusiveCounter<ThreadUnsafeCounterType>* ptr)
			{
				ptr->count++;
			}
			template<>
			static void increment(IntrusiveCounter<ThreadSafeCounterType>* ptr)
			{
				ptr->count.fetch_add(1, std::memory_order_relaxed);
			}

			template<typename CT, class BT>
			static void decrement(IntrusiveCounter<CT>* ptr);
			template<class BT>
			static void decrement(IntrusiveCounter<ThreadUnsafeCounterType>* ptr)
			{
				static_assert(sizeof(BT) != 0, "Type must be complete");
				ptr->count--;
				if (!count(ptr))
					delete static_cast<const BT*>(ptr);
			};
			template<class BT>
			static void decrement(IntrusiveCounter<ThreadSafeCounterType>* ptr)
			{
				static_assert(sizeof(BT) != 0, "Type must be complete");
				ptr->count.fetch_sub(1, std::memory_order_acq_rel);
				if (!count(ptr))
					delete static_cast<const BT*>(ptr);
			};

			template<typename CT>
			static uint64_t count(IntrusiveCounter<CT>* ptr);
			template<>
			static uint64_t count(IntrusiveCounter<ThreadUnsafeCounterType>* ptr)
			{
				return ptr->count;
			};
			template<>
			static uint64_t count(IntrusiveCounter<ThreadSafeCounterType>* ptr)
			{
				return ptr->count.load(std::memory_order_relaxed);
			};
		};
	}

	typedef intrusivePtrInternal::IntrusiveCounter<intrusivePtrInternal::ThreadUnsafeCounterType> IntrusiveCounter;
	typedef intrusivePtrInternal::IntrusiveCounter<intrusivePtrInternal::ThreadSafeCounterType  > IntrusiveCounterAtomic;

	template<class T>
	class IntrusivePtr
	{
		static_assert(std::is_base_of_v<IntrusiveCounter, T> != std::is_base_of_v<IntrusiveCounterAtomic,T>,
			"IntrusivePtr::T must derive from either IntrusiveCounter or IntrusiveCounterAtomic.");

	public:
		constexpr IntrusivePtr(T* ptr = nullptr, bool addRefence = true) noexcept
			: m_ptr(ptr)
		{
			if (m_ptr && addRefence)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		};

		constexpr IntrusivePtr(const IntrusivePtr& other) noexcept
			: m_ptr(other.m_ptr)
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		constexpr IntrusivePtr(IntrusivePtr&& other) noexcept
			: m_ptr(other.m_ptr)
		{
			other.m_ptr = nullptr;
		}

		constexpr ~IntrusivePtr() noexcept
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::decrement<T>(this->get());
		}

		constexpr IntrusivePtr& operator =(const IntrusivePtr& other) noexcept
		{
			IntrusivePtr(other).swap(*this);
			return *this;
		}

		constexpr IntrusivePtr& operator =(IntrusivePtr&& other) noexcept
		{
			IntrusivePtr(std::move(other)).swap(*this);
			return *this;
		}

		constexpr IntrusivePtr& operator =(T* other) noexcept
		{
			IntrusivePtr(other).swap(*this);
			return *this;
		}

		constexpr T* get() const noexcept
		{
			return m_ptr;
		}

		constexpr void swap(IntrusivePtr& other) noexcept
		{
			T* temp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = temp;
		}

		constexpr void reset(T* ptr = nullptr, bool addRefence = true) noexcept
		{
			IntrusivePtr(ptr, addRefence).swap(*this);
		}

		/// @brief This will return the detached raw pointer, but will not remove the reference.
		/// This means that the pointer can then be treated as a raw pointer.
		/// @brief Warning: As the reference count wont be decremented the automatic deletion of
		/// the pointer will not occure.
		constexpr T* detach() noexcept
		{
			T* temp = m_ptr;
			m_ptr = nullptr;
			return temp;
		}

		constexpr T& operator *() const noexcept
		{
			return *m_ptr;
		}

		constexpr T* operator ->() const noexcept
		{
			return m_ptr;
		}

		constexpr explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

	private:
		T* m_ptr;
	};

	template<class T, class O>
	constexpr bool operator==(const IntrusivePtr<T>& lhs, const IntrusivePtr<O>& rhs) noexcept
	{
		return lhs.get() == rhs.get();
	}

	template<class T, class O>
	constexpr bool operator!=(const IntrusivePtr<T>& lhs, const IntrusivePtr<O>& rhs) noexcept
	{
		return lhs.get() != rhs.get();
	}

	template<class T, class O>
	constexpr bool operator==(const IntrusivePtr<T>& lhs, O* rhs) noexcept
	{
		return lhs.get() == rhs;
	}

	template<class T, class O>
	constexpr bool operator!=(const IntrusivePtr<T>& lhs, O* rhs) noexcept
	{
		return lhs.get() != rhs;
	}

	template<class T, class O>
	constexpr bool operator==(T* lhs, const IntrusivePtr<O>& rhs) noexcept
	{
		return lhs == rhs.get();
	}

	template<class T, class O>
	constexpr bool operator!=(T* lhs, const IntrusivePtr<O>& rhs) noexcept
	{
		return lhs != rhs.get();
	}

	template<class T>
	constexpr bool operator==(const IntrusivePtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator!=(const IntrusivePtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() != nullptr;
	}

	template<class T>
	constexpr bool operator==(std::nullptr_t, const IntrusivePtr<T>& ptr) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator!=(std::nullptr_t, const IntrusivePtr<T>& ptr) noexcept
	{
		return ptr.get() != nullptr;
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> static_pointer_cast(const IntrusivePtr<O>& iptr) noexcept
	{
		return static_cast<T*>(iptr.get());
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> const_pointer_cast(const IntrusivePtr<O>& iptr) noexcept
	{
		return const_cast<T*>(iptr.get());
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> dynamic_pointer_cast(const IntrusivePtr<O>& iptr) noexcept
	{
		return dynamic_cast<T*>(iptr.get());
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> static_pointer_cast(IntrusivePtr<O>&& iptr) noexcept
	{
		return IntrusivePtr<T>(static_cast<T*>(iptr.detach()), false);
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> const_pointer_cast(IntrusivePtr<O>&& iptr) noexcept
	{
		return IntrusivePtr<T>(const_cast<T*>(iptr.detach()), false);
	}

	template<class T, class O>
	constexpr IntrusivePtr<T> dynamic_pointer_cast(IntrusivePtr<O>&& iptr) noexcept
	{
		T* temp = dynamic_cast<T*>(iptr.get());
		IntrusivePtr<T> newPtr(temp, false);
		if (temp)
			iptr.detach();
		return newPtr;
	}

#pragma endregion
#pragma region ScopedPtr

	template<class T>
	class ScopedPtr 
	{
	public:
		constexpr explicit ScopedPtr(T* ptr = nullptr) noexcept
			: m_ptr(ptr)
		{
		}

		constexpr ~ScopedPtr() noexcept
		{
			static_assert(sizeof(T) != 0, "Type must be complete");
			delete m_ptr;
			m_ptr = nullptr;
		}

		constexpr T* get() const noexcept
		{
			return m_ptr;
		}

		constexpr void swap(ScopedPtr& other) noexcept
		{
			T* temp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = temp;
		}

		constexpr void reset(T* ptr = nullptr) noexcept
		{
			ScopedPtr<T>(ptr).swap(*this);
		}

		/// @brief This will return the detached raw pointer.
		/// This means that the pointer can then be treated as a raw pointer.
		/// @brief Warning: As the pointer will now be detached automatic deletion of
		/// the pointer will not occure.
		constexpr T* detach() noexcept
		{
			T* temp = m_ptr;
			m_ptr = nullptr;
			return temp;
		}

		constexpr T& operator *() const noexcept
		{
			return *m_ptr;
		}

		constexpr T* operator ->() const noexcept
		{
			return m_ptr;
		}

		constexpr explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

	private:
		T* m_ptr;
	};

	template<class T>
	constexpr bool operator==(const ScopedPtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator==(std::nullptr_t, const ScopedPtr<T>& ptr) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator!=(const ScopedPtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() != nullptr;
	}

	template<class T>
	constexpr bool operator!=(std::nullptr_t, const ScopedPtr<T>& ptr) noexcept
	{
		return ptr.get() != nullptr;
	}

#pragma endregion
}