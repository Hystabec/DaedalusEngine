#pragma once

#include <atomic>
#include <type_traits>

// TO DO: Move buffer into this file as it is similarly a memory holder class
// add should probably also being included inside core

namespace daedalus {
#pragma region IntrusivePtr

	// consider changing the namespace name.
	// counters -> intrusive || counters -> intrusiveCounters
	// could make the namespace more clear for what its used for
	namespace counters {
		namespace internals { template<class> struct IntrusiveCounter; }

		struct ReferenceCounter
		{
			template<class> friend struct internals::IntrusiveCounter;
		protected:
			ReferenceCounter() = default;
		private:
			uint64_t count{ 0 };
		};

		struct AtomicReferenceCounter
		{
			template<class> friend struct internals::IntrusiveCounter;
		protected:
			AtomicReferenceCounter() = default;
		private:
			std::atomic<uint64_t> count{ 0 };
		};

		namespace internals {
			template<class baseT>
			struct IntrusiveCounter
			{
				static void increment(counters::ReferenceCounter* ptr) { ptr->count++; }
				static void decrement(counters::ReferenceCounter* ptr)
				{
					ptr->count--;
					if (!count(ptr))
						delete static_cast<const baseT*>(ptr);
				}
				static uint64_t count(counters::ReferenceCounter* ptr) { return ptr->count; }

				static void increment(counters::AtomicReferenceCounter* ptr) { ptr->count.fetch_add(1, std::memory_order_relaxed); }
				static void decrement(counters::AtomicReferenceCounter* ptr)
				{
					ptr->count.fetch_sub(1, std::memory_order_acq_rel);
					if (!count(ptr))
						delete static_cast<const baseT*>(ptr);
				}
				static uint64_t count(counters::AtomicReferenceCounter* ptr) { return ptr->count.load(std::memory_order_relaxed); }
			};
		}

	}

	template<class T>
	class IntrusivePtr
	{
		static_assert(std::is_base_of_v<counters::ReferenceCounter, T> != std::is_base_of_v<counters::AtomicReferenceCounter,T>,
			"IntrusivePtr::T must derive from either counters::ReferenceCounter or counters::AtomicReferenceCounter.");

	public:
		IntrusivePtr(T* ptr = nullptr) noexcept
			: m_ptr(ptr)
		{
			if (m_ptr)
				counters::internals::IntrusiveCounter<T>::increment(this->get());
		};

		IntrusivePtr(const IntrusivePtr& other) noexcept
			: m_ptr(other.m_ptr)
		{
			if (m_ptr)
				counters::internals::IntrusiveCounter<T>::increment(this->get());
		}

		~IntrusivePtr() noexcept
		{
			if (m_ptr)
				counters::internals::IntrusiveCounter<T>::decrement(this->get());
		}

		IntrusivePtr& operator =(IntrusivePtr&& other) noexcept
		{
			IntrusivePtr(std::move(other)).swap(*this);
			return *this;
		}

		IntrusivePtr& operator =(const IntrusivePtr& other) noexcept
		{
			IntrusivePtr(other).swap(*this);
			return *this;
		}

		T* get() noexcept
		{
			return m_ptr;
		}

		void swap(IntrusivePtr& other) noexcept
		{
			T* temp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = temp;
		}

		constexpr void reset(T* ptr = nullptr) noexcept
		{
			IntrusivePtr(ptr).swap(*this);
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
	constexpr bool operator==(const IntrusivePtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator==(std::nullptr_t, const IntrusivePtr<T>& ptr) noexcept
	{
		return ptr.get() == nullptr;
	}

	template<class T>
	constexpr bool operator!=(const IntrusivePtr<T>& ptr, std::nullptr_t) noexcept
	{
		return ptr.get() != nullptr;
	}

	template<class T>
	constexpr bool operator!=(std::nullptr_t, const IntrusivePtr<T>& ptr) noexcept
	{
		return ptr.get() != nullptr;
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