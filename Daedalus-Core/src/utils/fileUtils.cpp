#include "ddpch.h"
#include "fileUtils.h"

#include <fstream>

namespace daedalus::utils {

	static bool s_suppressOpErrors = false;

	std::string FileSystem::readFileString(const std::filesystem::path& filepath, bool* checkBool)
	{
		DD_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

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
				if (!s_suppressOpErrors)
					DD_CORE_LOG_ERROR("Could not read file '{}'", filepath);

				if (checkBool)
					(*checkBool) = false;
			}
			in.close();
		}
		else
		{
			if (!s_suppressOpErrors)
				DD_CORE_LOG_ERROR("Could not open file '{}'", filepath);

			if (checkBool)
				(*checkBool) = false;
		}

		s_suppressOpErrors = false;
		return result;
	}

	void FileSystem::writeFileString(const std::filesystem::path& filepath, const std::string& data, bool* checkBool)
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
			if (!s_suppressOpErrors)
				DD_CORE_LOG_ERROR("Could not write to file: {}", filepath);

			if (checkBool)
				(*checkBool) = false;
		}

		s_suppressOpErrors = false;
	}

	Buffer FileSystem::readFileBinary(const std::filesystem::path& filepath, bool* checkBool)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			if (!s_suppressOpErrors)
				DD_CORE_LOG_ERROR("Could not open file '{}'", filepath);

			s_suppressOpErrors = false;
			if (checkBool)
				(*checkBool) = false;
			return {};
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = (uint32_t)(end - stream.tellg());

		if (size == 0)
		{
			// File is empty
			s_suppressOpErrors = false;

			// checkBool = true as nothing when wrong
			// the file is just empty
			if (checkBool)
				(*checkBool) = true;

			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.as<char>(), size);
		stream.close();

		if (checkBool)
			(*checkBool) = true;

		s_suppressOpErrors = false;
		return buffer;
	}

	void FileSystem::writeFileBinary(const std::filesystem::path& filepath, Buffer data, bool* checkBool)
	{
		std::ofstream out(filepath, std::ios::out | std::ios::binary);
		if (out.is_open())
		{
			out.write(data.as<const char>(), data.size());
			out.flush();
			out.close();

			if (checkBool)
				(*checkBool) = true;
		}
		else
		{
			if (!s_suppressOpErrors)
				DD_CORE_LOG_ERROR("Could not write to file: {}", filepath);

			if (checkBool)
				(*checkBool) = false;
		}

		s_suppressOpErrors = false;
	}

	std::filesystem::file_time_type FileSystem::fileDateModified(const std::filesystem::path& filepath, bool* checkBool)
	{
		DD_PROFILE_FUNCTION();

		try
		{
			auto ftime = std::filesystem::last_write_time(filepath);

			if (checkBool)
				(*checkBool) = true;

			s_suppressOpErrors = false;
			return ftime;
		}
		catch (std::filesystem::filesystem_error& e)
		{
			if(!s_suppressOpErrors)
				DD_CORE_LOG_ERROR("Could not read file date modified: Error: {}, Path: {}", e.what(), filepath);
		}

		if (checkBool)
			(*checkBool) = false;

		s_suppressOpErrors = false;
		return std::filesystem::file_time_type();
	}

	std::filesystem::path FileSystem::fileExtensionToLower(const std::filesystem::path& extension)
	{
		// NOTE: This will probably not work with wchar strings aka std::filesystem::path. It also
		// might not be the best solution
		// TO DO: Revist this and check if there is a better method to do this
		std::string extensionStr = extension.string();
		std::transform(extensionStr.begin(), extensionStr.end(), extensionStr.begin(), ::tolower);
		s_suppressOpErrors = false; // cant log an error but to keep consistent
		return std::filesystem::path(extensionStr);
	}

	void FileSystem::suppressErrorsForNextOperation()
	{
		s_suppressOpErrors = true;
	}

}