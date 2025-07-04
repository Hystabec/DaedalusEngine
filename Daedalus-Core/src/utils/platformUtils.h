#pragma once

#include <string>

namespace daedalus::utils {

	class FileDialog
	{
	public:
		// Returns empty string if cancelled
		static std::string openFile(const char* filter);
		// Returns empty string if cancelled
		static std::string saveFile(const char* filter);
	};

}