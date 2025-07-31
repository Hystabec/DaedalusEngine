#pragma once

#include <string>
#include <filesystem>

namespace daedalus::utils {

	/// @brief Returns the file path to the file name argument.
	/// @brief While unbuild/using IDE will be in core project
	/// @brief When launching built .exe will be next to it
	static std::pair<std::filesystem::path, bool> get_core_resource_file_location(const std::string& file)
	{
        DD_PROFILE_FUNCTION();

#ifndef DD_DISTRO

        //when not in Distro build check in project files, for easier debugging

        std::string defaultLocStr = __FILE__;
        defaultLocStr.erase(defaultLocStr.find_last_of('\\'));
        defaultLocStr += "\\..\\..\\resources\\" + file;

        std::filesystem::path defaultPath(defaultLocStr);
        defaultPath = defaultPath.lexically_normal();

        // Try relative to the current working directory
        if (std::filesystem::exists(defaultPath))
        {
            return { defaultPath, true };
        }

#endif 

        // Check if next to executable
        if (std::filesystem::exists("resources\\" + file))
            return { "resources\\" + file, true };

#ifndef DD_DISTRO
        //doesnt support launching from different dir that executable
        DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("default2DShader.glsl not found in {} or {}", defaultPath.string(), "resources\\shaders\\default2DShader.glsl"));
#else
        DD_CORE_LOG_CRITICAL("default2DShader.glsl not found in {}", "resources\\shaders\\default2DShader.glsl");
        return { "", false };
#endif

        return { "", false };
	}

}