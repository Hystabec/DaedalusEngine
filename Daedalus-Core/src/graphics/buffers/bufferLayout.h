#pragma once

#include "ddpch.h"

namespace daedalus { namespace graphics { namespace buffers {

	namespace dataTypes
	{
		enum class BaseDataType
		{
			None = 0,
			Bool,
			Int, Float, Double
		};

		struct DataType
		{
			uint32_t Size;
			BaseDataType Type;
			uint32_t ComponentCount;

			DataType(uint32_t size, BaseDataType type, uint32_t componentCount)
				: Size(size), Type(type), ComponentCount(componentCount)
			{
			}
		};
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Offset;
		bool Normalized;

		BufferElement()
			: Name("None"), Offset(0), Normalized(false), m_BaseType(dataTypes::DataType(0, dataTypes::BaseDataType::None, 0))
		{
		}

		BufferElement(const dataTypes::DataType& type, const std::string& name, bool normalized = false)
			: Name(name), m_BaseType(type), Offset(0), Normalized(normalized)
		{
		}

		inline uint32_t getSize() const { return m_BaseType.Size; }
		inline uint32_t getComponentCount() const { return m_BaseType.ComponentCount; }
		inline dataTypes::BaseDataType getBaseDataType() const { return m_BaseType.Type; }

	private:
		dataTypes::DataType m_BaseType;
		friend class BufferLayout;
	};

	class BufferLayout
	{
	public:
		BufferLayout() 
		{
		}

		BufferLayout(const std::initializer_list<BufferElement>& element) 
			: m_elements(element)
		{
			calcOffsetStide();
		}

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline uint32_t getStide() const { return m_stride; }

	private:
		void calcOffsetStide()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				offset += element.m_BaseType.Size;
				m_stride += element.m_BaseType.Size;
			}
		}

	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};

} } }

#define DD_BUFFERS_INT   daedalus::graphics::buffers::dataTypes::DataType(sizeof(int),           daedalus::graphics::buffers::dataTypes::BaseDataType::Int,   1)
#define DD_BUFFERS_FLOAT daedalus::graphics::buffers::dataTypes::DataType(sizeof(float),         daedalus::graphics::buffers::dataTypes::BaseDataType::Float, 1)
#define DD_BUFFERS_BOOL  daedalus::graphics::buffers::dataTypes::DataType(sizeof(bool),          daedalus::graphics::buffers::dataTypes::BaseDataType::Bool,  1)
#define DD_BUFFERS_VEC2  daedalus::graphics::buffers::dataTypes::DataType(sizeof(float) * 2,     daedalus::graphics::buffers::dataTypes::BaseDataType::Float, 2)
#define DD_BUFFERS_VEC3  daedalus::graphics::buffers::dataTypes::DataType(sizeof(float) * 3,     daedalus::graphics::buffers::dataTypes::BaseDataType::Float, 3)
#define DD_BUFFERS_VEC4  daedalus::graphics::buffers::dataTypes::DataType(sizeof(float) * 4,     daedalus::graphics::buffers::dataTypes::BaseDataType::Float, 4)
#define DD_BUFFERS_VEC2I daedalus::graphics::buffers::dataTypes::DataType(sizeof(int) * 2,       daedalus::graphics::buffers::dataTypes::BaseDataType::Int,   2)
#define DD_BUFFERS_VEC3I daedalus::graphics::buffers::dataTypes::DataType(sizeof(int) * 3,       daedalus::graphics::buffers::dataTypes::BaseDataType::Int,   3)
#define DD_BUFFERS_VEC4I daedalus::graphics::buffers::dataTypes::DataType(sizeof(int) * 4,       daedalus::graphics::buffers::dataTypes::BaseDataType::Int,   4)
#define DD_BUFFERS_MAT4  daedalus::graphics::buffers::dataTypes::DataType(sizeof(float) * 4 * 4, daedalus::graphics::buffers::dataTypes::BaseDataType::Float, 4*4)