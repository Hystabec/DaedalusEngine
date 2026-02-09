#pragma once

#include <filesystem>

namespace daedalus::utils {

	// Uses wchar_t* as I plan for this to be used with wstring literals (L""), 
	// and it used wchar_t as it will save the extra conversion inside the functions.
	class FileDialog
	{
	public:
		// NOTE: storing wchar_t* instead of char* so that it wont need to
		// be converted when it needs to be used
		struct FileFilter
		{
			const wchar_t* fileTypeName = nullptr;
			const wchar_t* fileExtension = nullptr;
		};

		// Returns empty string if cancelled
		static std::filesystem::path openFile(FileFilter filter, const wchar_t* dialogTitle = nullptr);
		// Returns empty string if cancelled
		static std::filesystem::path saveFile(FileFilter filter, const wchar_t* defaultName, const wchar_t* dialogTitle = nullptr);
		// Returns empty string if cancelled
		static std::filesystem::path selectFolder(const wchar_t* dialogTitle = nullptr);
	};

}