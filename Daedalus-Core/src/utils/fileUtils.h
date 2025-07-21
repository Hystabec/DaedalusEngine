#pragma once

#include <fstream>

namespace daedalus { namespace utils {

	static std::string read_file(const std::string& filePath, bool* checkBool = nullptr)
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
				DD_CORE_LOG_ERROR("Could not read file '{}'", filePath);

				if (checkBool)
					(*checkBool) = false;
			}
			in.close();
		}
		else
		{
			DD_CORE_LOG_ERROR("Could not open file '{}'", filePath);

			if(checkBool)
				(*checkBool) = false;
		}

		return result;
	}

} }