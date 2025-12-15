#pragma once

#include <stdint.h>
#include <cstring>

namespace daedalus::utils {

	/// @brief Non-owning raw data buffer
	struct Buffer
	{
		Buffer() = default;

		Buffer(uint64_t size)
		{
			allocate(size);
		}

		Buffer(const void* data, uint64_t size)
			: m_data((uint8_t*)data), m_size(size)
		{
		}

		Buffer(const Buffer&) = default;

		static Buffer Copy(Buffer other)
		{
			Buffer result(other.m_size);
			memcpy(result.m_data, other.m_data, other.m_size);
			return result;
		}

		uint8_t*& data() { return m_data; }
		const uint8_t* const& data() const { return m_data; }
		uint64_t& size() { return m_size; }
		const uint64_t& size() const { return m_size; }

		void allocate(uint64_t size)
		{
			release();

			m_data = (uint8_t*)malloc(size);
			this->m_size = size;
		}

		void release()
		{
			free(m_data);
			m_data = nullptr;
			m_size = 0;
		}

		template<typename T>
		T* as()
		{
			return (T*)m_data;
		}

		operator bool() const
		{
			return (bool)m_data;
		}

	private:
		uint8_t* m_data = nullptr;
		uint64_t m_size = 0;
	};

	struct ScopedBuffer
	{
		ScopedBuffer(Buffer buffer)
			: m_buffer(buffer)
		{
		}

		ScopedBuffer(uint64_t size)
			: m_buffer(size)
		{
		}

		~ScopedBuffer()
		{
			m_buffer.release();
		}

		uint8_t* data() { return m_buffer.data(); }
		uint64_t size() const { return m_buffer.size(); }
		Buffer buffer() { return m_buffer; }

		template<typename T>
		T* as()
		{
			return m_buffer.as<T>();
		}

		operator bool() const
		{
			return m_buffer;
		}

	private:
		Buffer m_buffer;
	};

}