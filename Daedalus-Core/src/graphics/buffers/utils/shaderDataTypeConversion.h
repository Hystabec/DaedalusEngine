#pragma once

#include "../bufferLayout.h"

#ifdef DD_RENDER_USING_OPENGL
#include <GL/glew.h>

namespace daedalusCore { namespace graphics { namespace buffers {

	static GLenum Shader_Data_Type_To_Open_GL_Base_Type(dataTypes::BaseDataType baseDataType)
	{
		switch (baseDataType)
		{
		case dataTypes::BaseDataType::Bool:   return GL_BOOL;
		case dataTypes::BaseDataType::Int:    return GL_INT;
		case dataTypes::BaseDataType::Float:  return GL_FLOAT;
		case dataTypes::BaseDataType::Double: return GL_DOUBLE;
		}
		DD_CORE_ASSERT(false, "Unknown BaseDataType");
		return 0;
	}

	static GLenum Shader_Data_Type_To_Open_GL_Base_Type(dataTypes::DataType dataType)
	{
		return Shader_Data_Type_To_Open_GL_Base_Type(dataType.Type);
	}

} } }

#endif