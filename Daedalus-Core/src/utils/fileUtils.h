#pragma once

#include "buffer.h"
#include <filesystem>

// TO DO: Currently when outputting the filepath in error messages it get passed as a string,
// this limits extended character sets, look to implement or find a solution to std::formatter
// not having formats for wchar

namespace daedalus::utils {

	class FileSystem
	{
		// NOTE: You could argue that the checkBool is unecessary as you could just check the size of the
		// returned buffer/string. But I like the option of being able to check if something explicity when
		// wrong e.g. not being able to open the file
	public:
		
		static std::string readFileString(const std::filesystem::path& filepath, bool* checkBool = nullptr);
	
		static void writeFileString(const std::filesystem::path& filepath, const std::string& data, bool* checkBool = nullptr);

		static Buffer readFileBinary(const std::filesystem::path& filepath, bool* checkBool = nullptr);

		static void writeFileBinary(const std::filesystem::path& filepath, Buffer data, bool* checkBool = nullptr);

		/// @brief Used to check the date a file was last modified
		/// @brief Used for the old filewatching system
		/// @brief NOTE: Consider removing as this is no longer used anywhere
		static std::filesystem::file_time_type fileDateModified(const std::filesystem::path& filepath, bool* checkBool = nullptr);

		/// @brief This will stop errors from being logged (if they occure) for the next FileSystem operation
		/// @brief An example of use is when checking shaders:
		/// the cache file may or may not exist; if it doesnt, it is handled and the user doesnt need to know there was an error reading the file.
		static void suppressErrorsForNextOperation();
	};

}