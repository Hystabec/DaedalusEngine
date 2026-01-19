#pragma once

#include <atomic>
#include <type_traits>
#include <stdint.h>
#include <cstring>

namespace daedalus {
#pragma region Buffers

	struct ScopedBuffer;

	/// @brief Non-owning raw data buffer
	struct Buffer
	{
		friend struct ScopedBuffer;

		constexpr Buffer() noexcept = default;

		constexpr Buffer(uint64_t size) noexcept
		{
			allocate(size);
		}

		constexpr Buffer(void* data, uint64_t size) noexcept
			: m_data((uint8_t*)data), m_size(size)
		{
		}

		constexpr Buffer(const Buffer&) noexcept = default;

		constexpr Buffer(Buffer&&) noexcept = default;

		[[nodiscard]] static inline Buffer Copy(const Buffer& other) noexcept
		{
			Buffer result(other.m_size);
			memcpy(result.m_data, other.m_data, other.m_size);
			return result;
		}

		[[nodiscard]] constexpr uint8_t* data() noexcept { return m_data; }
		[[nodiscard]] constexpr const uint8_t* data() const noexcept { return m_data; }
		[[nodiscard]] constexpr uint64_t size() const noexcept { return m_size; }

		constexpr void allocate(uint64_t size) noexcept
		{
			release();
			m_data = new uint8_t[size];
			this->m_size = size;
		}

		constexpr void release() noexcept
		{
			if (m_data == nullptr)
				return;

			delete[] m_data;
			m_data = nullptr;
			m_size = 0;
		}

		constexpr void swap(Buffer& other) noexcept
		{
			Buffer temp(*this);
			*this = other;
			other = temp;
		}

		constexpr void reset(Buffer&& other = Buffer()) noexcept
		{
			release();
			swap(other);
		}

		template<typename T>
		[[nodiscard]] constexpr T* as() noexcept
		{
			return (T*)m_data;
		}

		[[nodiscard]] constexpr operator bool() const noexcept
		{
			return (bool)m_data;
		}

	private:
		// Private as using this would most likely leave the memory hanging
		constexpr Buffer& operator =(const Buffer& other) noexcept
		{
			m_data = other.m_data;
			m_size = other.m_size;
			return *this;
		}

	private:
		uint8_t* m_data = nullptr;
		uint64_t m_size = 0;
	};

	struct ScopedBuffer
	{
		constexpr ScopedBuffer() noexcept
		{ }

		constexpr ScopedBuffer(Buffer&& buffer) noexcept
			: m_buffer(buffer)
		{ }

		inline ScopedBuffer(uint64_t size) noexcept
			: m_buffer(size)
		{ }

		constexpr ScopedBuffer(void* data, uint64_t size) noexcept
			: m_buffer(Buffer((uint8_t*)data, size))
		{ }

		inline ~ScopedBuffer() noexcept
		{
			m_buffer.release();
		}

		[[nodiscard]] constexpr uint8_t* data() noexcept { return m_buffer.data(); }
		[[nodiscard]] constexpr const uint8_t* data() const noexcept { return m_buffer.data(); }
		[[nodiscard]] constexpr uint64_t size() const noexcept { return m_buffer.size(); }
		[[nodiscard]] constexpr const Buffer& buffer() const noexcept { return m_buffer; }

		constexpr void swap(ScopedBuffer& other) noexcept
		{
			m_buffer.swap(other.m_buffer);
		}

		constexpr void reset(ScopedBuffer&& other = ScopedBuffer()) noexcept 
		{
			other.swap(*this);
		}

		/// @brief This will return the detached "raw" buffer.
		/// @brief Warning: As the buffer will now be detached automatic deletion of
		/// the buffer will not occure.
		[[nodiscard("Losing track of the detached buffer could cause a memory leak")]] constexpr Buffer detach() noexcept
		{
			Buffer temp = m_buffer;
			m_buffer = Buffer();
			return temp;
		}

		template<typename T>
		[[nodiscard]] constexpr T* as() noexcept
		{
			return m_buffer.as<T>();
		}

		constexpr operator bool() const noexcept
		{
			return m_buffer;
		}

	private:
		Buffer m_buffer;
	};

#pragma endregion
#pragma region IntrusivePtr

	namespace intrusivePtrInternal {

		// foward declare
		struct IntrusiveCounterInternal;

		typedef uint64_t ThreadUnsafeCounterType;
		typedef std::atomic<uint64_t> ThreadSafeCounterType;

		struct IntrusiveCounterThreadSafe
		{
			friend struct IntrusiveCounterInternal;
		protected:
			constexpr IntrusiveCounterThreadSafe() noexcept = default;
		private:
			ThreadSafeCounterType count{ 0 };
		};

		struct IntrusiveCounterThreadUnsafe
		{
			friend struct IntrusiveCounterInternal;
		protected:
			constexpr IntrusiveCounterThreadUnsafe() noexcept = default;
		private:
			ThreadUnsafeCounterType count{ 0 };
		};

		struct IntrusiveCounterInternal
		{
			static inline void increment(IntrusiveCounterThreadUnsafe* ptr) noexcept
			{
				ptr->count++;
			}
			static inline void increment(IntrusiveCounterThreadSafe* ptr) noexcept
			{
				ptr->count.fetch_add(1, std::memory_order_relaxed);
			}

			template<class BT>
			static inline void decrement(IntrusiveCounterThreadUnsafe* ptr) noexcept
			{
				static_assert(sizeof(BT) != 0, "Type must be complete");
				ptr->count--;
				if (!count(ptr))
					delete static_cast<const BT*>(ptr);
			};
			template<class BT>
			static inline void decrement(IntrusiveCounterThreadSafe* ptr) noexcept
			{
				static_assert(sizeof(BT) != 0, "Type must be complete");
				ptr->count.fetch_sub(1, std::memory_order_acq_rel);
				if (!count(ptr))
					delete static_cast<const BT*>(ptr);
			};

			static inline uint64_t count(IntrusiveCounterThreadUnsafe* ptr) noexcept
			{
				return ptr->count;
			};
			static inline uint64_t count(IntrusiveCounterThreadSafe* ptr) noexcept
			{
				return ptr->count.load(std::memory_order_relaxed);
			};
		};
	}

	typedef intrusivePtrInternal::IntrusiveCounterThreadUnsafe IntrusiveCounter;
	typedef intrusivePtrInternal::IntrusiveCounterThreadSafe   IntrusiveCounterAtomic;

	template<class T>
	class IntrusivePtr
	{
	public:
		constexpr IntrusivePtr() noexcept
			: m_ptr(nullptr)
		{
		}

		constexpr IntrusivePtr(std::nullptr_t) noexcept
			: m_ptr(nullptr)
		{
		}

		constexpr explicit IntrusivePtr(T* ptr) noexcept
			: m_ptr(ptr)
		{
			static_assert(std::is_base_of_v<IntrusiveCounter, T> != std::is_base_of_v<IntrusiveCounterAtomic, T>,
				"IntrusivePtr::Type must derive from either IntrusiveCounter or IntrusiveCounterAtomic.");
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		constexpr IntrusivePtr(const IntrusivePtr& right) noexcept
			: m_ptr(right.get())
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		constexpr IntrusivePtr(IntrusivePtr&& right) noexcept
			: m_ptr(right.get())
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		template<class T2, std::enable_if_t<std::disjunction_v<std::is_assignable<T, T2>, std::is_convertible<T2, T>>, int> = 0>
		constexpr IntrusivePtr(const IntrusivePtr<T2>& right) noexcept
			: m_ptr((T*)right.get())
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		template<class T2, std::enable_if_t<std::disjunction_v<std::is_assignable<T, T2>, std::is_convertible<T2, T>>, int> = 0>
		constexpr IntrusivePtr(IntrusivePtr<T2>&& right) noexcept
			: m_ptr((T*)right.get())
		{
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::increment(this->get());
		}

		constexpr ~IntrusivePtr() noexcept
		{
			static_assert(std::is_base_of_v<IntrusiveCounter, T> != std::is_base_of_v<IntrusiveCounterAtomic, T>,
				"IntrusivePtr::Type must derive from either IntrusiveCounter or IntrusiveCounterAtomic.");
			if (m_ptr)
				intrusivePtrInternal::IntrusiveCounterInternal::decrement<T>(this->get());
		}

		[[nodiscard]] constexpr T* get() const noexcept
		{
			return m_ptr;
		}

		constexpr void swap(IntrusivePtr& other) noexcept
		{
			T* temp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = temp;
		}

		constexpr void reset() noexcept
		{
			IntrusivePtr(nullptr).swap(*this);
		}

		constexpr void reset(T* ptr, bool addRefence = true) noexcept
		{
			IntrusivePtr(ptr, addRefence).swap(*this);
		}

		/// @brief This will return the detached raw pointer, but will not remove the reference.
		/// This means that the pointer can then be treated as a raw pointer.
		/// @brief Warning: As the reference count wont be decremented the automatic deletion of
		/// the pointer will not occure.
		[[nodiscard("Losing track of the detached pointer could cause a memory leak")]] constexpr T* detach() noexcept
		{
			T* temp = m_ptr;
			m_ptr = nullptr;
			return temp;
		}

		constexpr IntrusivePtr& operator=(const IntrusivePtr& right) noexcept
		{
			IntrusivePtr(right).swap(*this);
			return *this;
		}

		constexpr IntrusivePtr& operator=(IntrusivePtr&& right) noexcept
		{
			right.swap(*this);
			return *this;
		}

		template<class T2>
		constexpr IntrusivePtr& operator=(const IntrusivePtr<T2>& right) noexcept
		{
			IntrusivePtr(right).swap(*this);
			return *this;
		}

		template<class T2>
		constexpr IntrusivePtr& operator=(IntrusivePtr<T2>&& right) noexcept
		{
			right.swap(*this);
			return *this;
		}

		[[nodiscard]] constexpr T& operator *() const noexcept
		{
			return *m_ptr;
		}

		[[nodiscard]] constexpr T* operator ->() const noexcept
		{
			return m_ptr;
		}

		constexpr explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

	private:
		template<class>
		friend class IntrusivePtr;

		T* m_ptr;
	};

	template<class T, class... Args>
	[[nodiscard]] constexpr IntrusivePtr<T> make_intrusive_ptr(Args&&... args) noexcept
	{
		return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
	}

	template<class T>
	constexpr bool operator==(const IntrusivePtr<T>& left, std::nullptr_t) noexcept
	{
		return left.get() == nullptr;
	}

	template<class T>
	constexpr bool operator!=(const IntrusivePtr<T>& left, std::nullptr_t) noexcept
	{
		return left.get() != nullptr;
	}

	template<class T>
	constexpr bool operator==(std::nullptr_t, const IntrusivePtr<T>& right) noexcept
	{
		return nullptr == right.get();
	}

	template<class T>
	constexpr bool operator!=(std::nullptr_t, const IntrusivePtr<T>& right) noexcept
	{
		return nullptr != right.get();
	}

	template<class T, class T2>
	constexpr bool operator==(const IntrusivePtr<T>& left, const IntrusivePtr<T2>& right) noexcept
	{
		return left.get() == right.get();
	}

	template<class T, class T2>
	constexpr bool operator!=(const IntrusivePtr<T>& left, const IntrusivePtr<T2>& right) noexcept
	{
		return left.get() != right.get();
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> static_pointer_cast(const IntrusivePtr<T2>& other) noexcept
	{
		return IntrusivePtr<T>(static_cast<T*>(other.get()));
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> static_pointer_cast(IntrusivePtr<T2>&& other) noexcept
	{
		return IntrusivePtr<T>(static_cast<T*>(other.get()));
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> const_pointer_cast(const IntrusivePtr<T2>& other) noexcept
	{
		return IntrusivePtr<T>(const_cast<T*>(other.get()));
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> const_pointer_cast(IntrusivePtr<T2>&& other) noexcept
	{
		return IntrusivePtr<T>(const_cast<T*>(other.get()));
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> dynamic_pointer_cast(const IntrusivePtr<T2>& other) noexcept
	{
		return IntrusivePtr<T>(dynamic_cast<T*>(other.get()));
	}

	template<class T, class T2>
	[[nodiscard]] constexpr IntrusivePtr<T> dynamic_pointer_cast(IntrusivePtr<T2>&& other) noexcept
	{
		return IntrusivePtr<T>(dynamic_cast<T*>(other.get()));
	}

#pragma endregion
#pragma region ScopedPtr

	/// @brief ScopedPtr "owns" a memory location so when the ScopedPtr object falls
	/// out of scope the memory is "cleaned up".
	/// 
	/// @brief A memory location can only be "owned" be 1 ScopedPtr so if a ScopedPtr
	/// is construct using another the pevious ScopedPtr will no longer be valid. Same
	/// applies for using reset().
	/// 
	/// @brief WARNING: If you get() the underlying "raw" pointer then either delete
	/// it or use it to make a new ScopedPtr undefined behaviour will be encountered.
	template<class T>
	class ScopedPtr
	{
	public:
		constexpr ScopedPtr() noexcept
			: m_ptr(nullptr)
		{
		}

		constexpr ScopedPtr(std::nullptr_t) noexcept
			: m_ptr(nullptr)
		{
		}

		constexpr explicit ScopedPtr(T* ptr) noexcept
			: m_ptr(ptr)
		{
		}

		constexpr ScopedPtr(ScopedPtr&& right) noexcept
			: m_ptr(right.detach())
		{
		}

		template<class T2, std::enable_if_t<std::disjunction_v<std::is_assignable<T, T2>, std::is_convertible<T2, T>>, int> = 0>
		constexpr ScopedPtr(ScopedPtr<T2>&& right) noexcept
			: m_ptr((T*)right.detach())
		{
		}

		constexpr ~ScopedPtr() noexcept
		{
			static_assert(sizeof(T) != 0, "Type must be complete");
			if (m_ptr)
			{
				delete m_ptr;
				m_ptr = nullptr;
			}
		}

		[[nodiscard]] constexpr T* get() const noexcept
		{
			return m_ptr;
		}

		constexpr void reset(T* ptr = nullptr) noexcept
		{
			ScopedPtr(ptr).swap(*this);
		}

		constexpr void swap(ScopedPtr& right) noexcept
		{
			std::swap(this->m_ptr, right.m_ptr);
		}

		/// @brief This will return the detached raw pointer.
		/// This means that the pointer can then be treated as a raw pointer.
		/// @brief Warning: As the pointer will now be detached automatic deletion of
		/// the pointer will not occure.
		[[nodiscard("Losing track of the detached pointer will cause a memory leak")]] constexpr T* detach() noexcept
		{
			T* holder = m_ptr;
			m_ptr = nullptr;
			return holder;
		}

		[[nodiscard]] constexpr T& operator *() const noexcept
		{
			return *m_ptr;
		}

		[[nodiscard]] constexpr T* operator->() const noexcept
		{
			return m_ptr;
		}

		constexpr explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

		constexpr ScopedPtr& operator=(std::nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		constexpr ScopedPtr& operator=(ScopedPtr&& right) noexcept
		{
			reset(right.detach());
			return *this;
		}

		template<class T2, std::enable_if_t<std::disjunction_v<std::is_assignable<T, T2>, std::is_convertible<T2, T>>, int> = 0>
		constexpr ScopedPtr& operator=(ScopedPtr<T2>&& right) noexcept
		{
			reset(right.detach());
			return *this;
		}

		ScopedPtr(const ScopedPtr&) = delete;
		ScopedPtr& operator=(const ScopedPtr&) = delete;

	private:
		template<class>
		friend class ScopedPtr;

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

	template<class T, class T2>
	constexpr bool operator==(const ScopedPtr<T>& left, const ScopedPtr<T2>& right) noexcept
	{
		return left.get() == right.get();
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

	template<class T, class T2>
	constexpr bool operator!=(const ScopedPtr<T>& left, const ScopedPtr<T2>& right) noexcept
	{
		return left.get() != right.get();
	}

	template<class T, class... Args>
	[[nodiscard]] constexpr ScopedPtr<T> make_scoped_ptr(Args&&... args) noexcept
	{
		return ScopedPtr<T>(new T(std::forward<Args>(args)...));
	}

#pragma endregion
}