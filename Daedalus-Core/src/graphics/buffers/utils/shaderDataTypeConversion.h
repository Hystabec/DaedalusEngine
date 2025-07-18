#pragma once

#include "../bufferLayout.h"

#ifdef DD_RENDER_USING_OPENGL
#include <GL/glew.h>

namespace daedalus { namespace graphics { namespace buffers {

	static GLenum shader_data_type_to_Open_GL_base_type(dataTypes::BaseDataType baseDataType)
	{
		switch (baseDataType)
		{
		case dataTypes::BaseDataType::Bool:   return GL_BOOL;
		case dataTypes::BaseDataType::uInt:   return GL_UNSIGNED_INT;
		case dataTypes::BaseDataType::Int:    return GL_INT;
		case dataTypes::BaseDataType::Float:  return GL_FLOAT;
		case dataTypes::BaseDataType::Double: return GL_DOUBLE;
		}
		DD_CORE_ASSERT(false, "Unknown BaseDataType");
		return 0;
	}

	static GLenum shader_data_type_to_Open_GL_base_type(dataTypes::DataType dataType)
	{
		return shader_data_type_to_Open_GL_base_type(dataType.Type);
	}

} } }

#endif