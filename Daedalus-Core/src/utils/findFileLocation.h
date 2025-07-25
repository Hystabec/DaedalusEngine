#pragma once

#include <string>

namespace daedalus::utils {

	/// @brief Returns the file path to the file name argument.
	/// @brief While unbuild/using IDE will be in core project
	/// @brief When launching built .exe will be next to it
	static std::pair<std::string, bool> get_core_resource_file_location(const std::string& file)
	{
        DD_PROFILE_FUNCTION();

#ifndef DD_DISTRO

        //when not in Distro build check in project files, for easier debugging

        std::string defaultLoc = __FILE__;
        defaultLoc.erase(defaultLoc.find_last_of('\\'));
        defaultLoc += "\\..\\..\\resources\\" + file;

        // Try relative to the current working directory
        if (std::filesystem::exists((defaultLoc)))
        {
            return { defaultLoc, true };
        }

#endif 

        // Check if next to executable
        if (std::filesystem::exists("resources\\" + file))
            return { "resources\\" + file, true };

#ifndef DD_DISTRO
        //doesnt support launching from different dir that executable
        DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("default2DShader.glsl not found in {} or {}", defaultLoc, "resources\\shaders\\default2DShader.glsl"));
#else
        DD_CORE_LOG_CRITICAL("default2DShader.glsl not found in {}", "resources\\shaders\\default2DShader.glsl");
        return { "", false };
#endif

        return { "", false };
	}

}