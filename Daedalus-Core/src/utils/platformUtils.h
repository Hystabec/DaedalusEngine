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

	class ChildProcess {
	public:
		virtual ~ChildProcess() {};

		virtual bool isValid() = 0;
		virtual bool isRunning() = 0;
		virtual void killProcess() = 0;

		// Returns true if the process finished before reaching the timeout
		virtual bool waitForProcess(unsigned long timeoutMS = 0xFFFFFFFF) = 0;

		friend ScopedPtr<ChildProcess> create_child_process(const std::string&);
	};

	ScopedPtr<ChildProcess> create_child_process(const std::string& args);
}