#pragma once

#include <fstream>

namespace daedalusCore { namespace utils {

	static std::string read_file(const std::string& filePath, bool* checkBool = nullptr)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();

			if(checkBool)
				(*checkBool) = true;
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