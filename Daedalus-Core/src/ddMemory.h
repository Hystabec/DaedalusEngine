#pragma once

// TO DO: Move buffer into this file as it is similarly a memory holder class
// add should probably also being included inside core

namespace daedalus {
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
#pragma region IntrusivePtr

	template<class T>
	class IntrusivePtr
	{

	};

#pragma endregion
}