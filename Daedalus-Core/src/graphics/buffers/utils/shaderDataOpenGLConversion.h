#pragma once
#ifdef DD_RENDER_USING_OPENGL
#include "../bufferLayout.h"
#include <GL/glew.h>

namespace daedalusCore { namespace graphics { namespace buffers {

	static GLenum Shader_Data_Type_To_Open_GL_Base_Type(DataTypes::BaseDataType baseDataType)
	{
		switch (baseDataType)
		{
		case DataTypes::BaseDataType::Bool:   return GL_BOOL;
		case DataTypes::BaseDataType::Int:    return GL_INT;
		case DataTypes::BaseDataType::Float:  return GL_FLOAT;
		case DataTypes::BaseDataType::Double: return GL_DOUBLE;
		}
		DD_CORE_ASSERT(false, "Unknown BaseDataType");
		return 0;
	}

	static GLenum Shader_Data_Type_To_Open_GL_Base_Type(DataTypes::DataType dataType)
	{
		return Shader_Data_Type_To_Open_GL_Base_Type(dataType.Type);
	}

} } }

#endif