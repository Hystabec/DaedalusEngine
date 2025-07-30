#pragma once

#include <fstream>
#include <filesystem>

// TO DO: Currently when outputting the filepath in error messages it get passed as a string,
// this limits extended character sets, look to implement or find a solution to std::formatter
// not having formats for wchar

namespace daedalus::utils {

	/// @brief 'noErrors' with stop the console logs. An example of use is when checking shaders:
	/// the cache file may or may not exist; if it doesnt, it is handled and the user doesnt need to know the was an error reading the file.
	static std::string read_file_string(const std::filesystem::path& filePath, bool* checkBool = nullptr, bool noErrors = false)
	{
		DD_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);

			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());

				if (checkBool)
					(*checkBool) = true;
			}
			else
			{
				if(!noErrors)
					DD_CORE_LOG_ERROR("Could not read file '{}'", filePath.string());

				if (checkBool)
					(*checkBool) = false;
			}
			in.close();
		}
		else
		{
			if (!noErrors)
				DD_CORE_LOG_ERROR("Could not open file '{}'", filePath.string());

			if(checkBool)
				(*checkBool) = false;
		}

		return result;
	}
	
	static void write_file_string(const std::filesystem::path& filepath, const std::string& data, bool* checkBool = nullptr)
	{
		DD_PROFILE_FUNCTION();

		std::ofstream out(filepath, std::ios::out | std::ios::binary);
		if (out.is_open())
		{
			out.write((const char*)data.data(), data.size() * sizeof(char));
			out.flush();
			out.close();

			if (checkBool)
				(*checkBool) = true;
		}
		else
		{
			DD_CORE_LOG_ERROR("Could not write to file: {}", filepath.string());

			if (checkBool)
				(*checkBool) = false;
		}
	}

	static std::filesystem::file_time_type file_data_modified(const std::filesystem::path& filepath, bool* checkBool = nullptr)
	{
		DD_PROFILE_FUNCTION();

		try
		{
			auto ftime = std::filesystem::last_write_time(filepath);

			if (checkBool)
				(*checkBool) = true;

			return ftime;
		}
		catch (std::filesystem::filesystem_error& e)
		{
			DD_CORE_LOG_ERROR("Could not read file data modified: Error: {}, Path: {}", e.what(), filepath.string());
		}

		if (checkBool)
			(*checkBool) = false;
		
		return std::filesystem::file_time_type();
	}

}