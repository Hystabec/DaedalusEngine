#include "ddpch.h"
#include "rendererUtils.h"

#include <filesystem>

namespace daedalusCore::graphics {
namespace renderer2DUtils {

	/// TO DO: I could add a post build command to premake that copies the "default2DShader.glsl"
    /// into the (or creates if path doesnt already exist) "resources\shaders" path next too the compiled .exe
 
	std::string get_default_shader_path(bool* checkBool)
	{
        DD_PROFILE_FUNCTION();

        // There might be a more efficient way to do this but it find the default shader relative to this file
        // so if this file or this folder move this will no longer work
        std::string defaultShaderLoc = __FILE__;
        defaultShaderLoc.erase(defaultShaderLoc.find_last_of('\\'));
        defaultShaderLoc += "\\..\\..\\..\\..\\resources\\shaders\\default2DShader.glsl";

        // Try relative to the current working directory
        if (std::filesystem::exists((defaultShaderLoc)))
        {
            (*checkBool) = true;
            return defaultShaderLoc;
        }

        // Check if next to executable
        if (std::filesystem::exists("resources\\shaders\\default2DShader.glsl"))
        {
            (*checkBool) = true;
            return "resources\\shaders\\default2DShader.glsl";
        }

        //doesnt support launching from different dir that executable
        DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("default2DShader.glsl not found in {} or {}", defaultShaderLoc, "resources\\shaders\\default2DShader.glsl"));
        (*checkBool) = false;
        return "";
	}

}
}