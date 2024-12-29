#pragma once

#include "ddpch.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	namespace DataTypes
	{
		enum class BaseDataType
		{
			None = 0,
			Bool,
			Int, Float, Double
		};

		struct DataType
		{
			const uint32_t Size;
			const BaseDataType Type;
			const uint32_t ComponentCount;

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

		BufferElement(const DataTypes::DataType& type, const std::string& name, bool normalized = false)
			: Name(name), m_BaseType(type), Offset(0), Normalized(normalized)
		{
		}

		inline uint32_t getSize() const { m_BaseType.Size; }
		inline uint32_t getComponentCount() const { m_BaseType.ComponentCount; }
		inline DataTypes::BaseDataType getBaseDataType() const { return m_BaseType.Type; }

	private:
		DataTypes::DataType m_BaseType;
		friend class BufferLayout;
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
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
	public:
		BufferLayout(std::initializer_list<BufferElement>& element) 
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
	};

} } }

#define DD_BUFFERS_INT   DataTypes::DataType(sizeof(int),           BaseDataType::Int,   1)
#define DD_BUFFERS_FLOAT DataTypes::DataType(sizeof(float),         BaseDataType::Float, 1)
#define DD_BUFFERS_BOOL  DataTypes::DataType(sizeof(bool),          BaseDataType::Bool,  1)
#define DD_BUFFERS_VEC2  DataTypes::DataType(sizeof(float) * 2,     BaseDataType::Float, 2)
#define DD_BUFFERS_VEC3  DataTypes::DataType(sizeof(float) * 3,     BaseDataType::Float, 3)
#define DD_BUFFERS_VEC4  DataTypes::DataType(sizeof(float) * 4,     BaseDataType::Float, 4)
#define DD_BUFFERS_VEC2I DataTypes::DataType(sizeof(int) * 2,       BaseDataType::Int,   2)
#define DD_BUFFERS_VEC3I DataTypes::DataType(sizeof(int) * 3,       BaseDataType::Int,   3)
#define DD_BUFFERS_VEC4I DataTypes::DataType(sizeof(int) * 4,       BaseDataType::Int,   4)
#define DD_BUFFERS_MAT4  DataTypes::DataType(sizeof(float) * 4 * 4, BaseDataType::Float, 4*4)