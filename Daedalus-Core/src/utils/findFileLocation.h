#pragma once

#include <string>
#include <filesystem>

namespace daedalus::utils {



	/// @brief Returns the file path to the file name argument.
	/// @brief While unbuild/using IDE will be in core project
	/// @brief When launching built .exe will be next to it
	static std::pair<std::filesystem::path, bool> get_core_file_location(const std::filesystem::path& file)
	{
        // NOTE: This function is currently (19.08.25) only used in constuction/init functions
        // if this function need to be called regularly, it should be made into a constexpr to remove any
        // preformance overhead

        DD_PROFILE_FUNCTION();

#ifndef DD_DISTRO

        //when not in Distro build check in project files, for easier debugging

        std::filesystem::path pathRelativeToThisFile = "\\..\\..\\" / file;

        std::filesystem::path defaultPath = __FILE__;
        defaultPath = defaultPath.parent_path();
        defaultPath += pathRelativeToThisFile;
        defaultPath = defaultPath.lexically_normal();

        // Try relative to the current working directory
        if (std::filesystem::exists(defaultPath))
        {
            return { defaultPath, true };
        }

#endif 

        // Check if next to executable
        if (std::filesystem::exists(file))
            return { file, true };

#ifndef DD_DISTRO
        //doesnt support launching from different dir that executable
        DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("{} not found in {} or {}", file.string(), defaultPath.string(), file.string()));
#else
        DD_CORE_LOG_CRITICAL("{} not found in {}", file.string(), file.string());
        return { "", false };
#endif

        return { "", false };
	}

}