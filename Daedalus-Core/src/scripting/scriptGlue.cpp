#include "ddpch.h"
#include "scriptGlue.h"

#include "maths/vec3.h"

#include <mono/jit/jit.h>

namespace daedalus::scripting {

#define DD_ADD_INTERNAL_CALL(FuncName) mono_add_internal_call("daedalus.InternalCalls::" #FuncName, FuncName)

	static void nativeLog(MonoString* string, int param)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		DD_CORE_LOG_INFO("{}, {}", str, param);
	}

	static void nativeLogVector3(maths::Vec3* param, maths::Vec3* outResult)
	{
		DD_CORE_LOG_INFO(*param);

		*outResult = *param + maths::Vec3(1.5f);
	}

	void ScriptGlue::registerFunctions()
	{
		DD_ADD_INTERNAL_CALL(nativeLog);
		DD_ADD_INTERNAL_CALL(nativeLogVector3);
	}

}