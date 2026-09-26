#include "ddpch.h"
#include "utils/platformUtils.h"

#include "application/applicationCore.h"
#include "platformSpecific/windows/windowsWindow.h"

#include <ShlObj.h>

#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

namespace daedalus::utils {

	namespace helpers {

		struct DialogOptions
		{
			bool isSaving = true;
			const wchar_t* defaultFileName = nullptr;
			FILEOPENDIALOGOPTIONS openflags = 0;
			FileDialog::FileFilter filters;
		};

		static std::filesystem::path dialog_function_base(const DialogOptions& options, const wchar_t* titleMsg)
		{
			std::filesystem::path returnResult;
			HRESULT hr;
			IFileDialog* fileDialog = nullptr;

			// needs to be done before use for the thread, could add init function?
			// could profile to see if it takes a long time, but also might just be a waste to do so
			hr = CoInitializeEx(nullptr,
				COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

			if (FAILED(hr)) {
				return L"";
			}

			hr = CoCreateInstance(options.isSaving ? CLSID_FileSaveDialog : CLSID_FileOpenDialog,
				nullptr,
				CLSCTX_ALL,
				options.isSaving ? IID_IFileSaveDialog : IID_IFileOpenDialog,
				reinterpret_cast<void**>(&fileDialog));

			if (SUCCEEDED(hr)) {
				if (titleMsg != nullptr)
					hr = fileDialog->SetTitle(titleMsg);
				if (SUCCEEDED(hr)) {
					FILEOPENDIALOGOPTIONS dwFlags;
					hr = fileDialog->GetOptions(&dwFlags);
					if (SUCCEEDED(hr)) {
						hr = fileDialog->SetOptions(dwFlags | options.openflags);
						if (SUCCEEDED(hr)) {
							if (options.filters.fileTypeName != nullptr && options.filters.fileExtension != nullptr) {
								// NOTE: currently only supports 1 filter
								COMDLG_FILTERSPEC rgSpec[] = { options.filters.fileTypeName, options.filters.fileExtension };
								hr = fileDialog->SetFileTypes(1, rgSpec);
							}
							if (SUCCEEDED(hr)) {
								if (options.defaultFileName != nullptr)
									hr = fileDialog->SetFileName(options.defaultFileName);
								if (SUCCEEDED(hr)) {
									hr = fileDialog->Show(glfwGetWin32Window((GLFWwindow*)Application::get().getWindow()->getNativeWindow())); // this (nullptr) would become the window handle?
									if (SUCCEEDED(hr)) {
										IShellItem* pItem = nullptr;
										hr = fileDialog->GetResult(&pItem);
										if (SUCCEEDED(hr)) {
											PWSTR pszFilePath = nullptr;
											hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
											if (SUCCEEDED(hr)) {
												returnResult = std::filesystem::path(pszFilePath);
												CoTaskMemFree(pszFilePath);
											}
											pItem->Release();
										}
									}
								}
							}
							fileDialog->Release();
						}
					}
				}
			}

			// needs to be done after use for the thread, could add destroy function?
			CoUninitialize();
			return returnResult;
		}
	}

	std::filesystem::path FileDialog::openFile(FileFilter filter, const wchar_t* dialogTitle)
	{
		helpers::DialogOptions options;
		options.isSaving = false;
		options.openflags = FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST;
		options.filters = filter;
		return helpers::dialog_function_base(options, dialogTitle);
	}

	std::filesystem::path FileDialog::saveFile(FileFilter filter, const wchar_t* defaultName, const wchar_t* dialogTitle)
	{
		helpers::DialogOptions options;
		options.isSaving = true;
		options.defaultFileName = defaultName;
		options.filters = filter;
		return helpers::dialog_function_base(options, dialogTitle);
	}

	std::filesystem::path FileDialog::selectFolder(const wchar_t* dialogTitle)
	{
		helpers::DialogOptions options;
		options.isSaving = false;
		options.openflags = FOS_PICKFOLDERS | FOS_PATHMUSTEXIST;
		return helpers::dialog_function_base(options, dialogTitle);
	}

	class WindowsChildProcess : public ChildProcess {
	public:
		WindowsChildProcess() = default;

		WindowsChildProcess(WindowsChildProcess&& other) noexcept : 
			m_pi(other.m_pi), m_hasClosed(other.m_hasClosed) {
			other.m_pi = PROCESS_INFORMATION();
			other.m_hasClosed = true;
		}

		~WindowsChildProcess() override {
			killProcess();
		}

		bool isValid() override {
			return m_valid;
		}

		bool isRunning() override {
			return WaitForSingleObject(m_pi.hProcess, 0) == WAIT_TIMEOUT;
		}

		void killProcess() override {
			if (!m_hasClosed)
			{
				TerminateProcess(m_pi.hProcess, 0);

				CloseHandle(m_pi.hProcess);
				CloseHandle(m_pi.hThread);
				m_hasClosed = true;
			}
		}

		bool waitForProcess(unsigned long timeoutMS) override {
			return WaitForSingleObject(m_pi.hProcess, timeoutMS) != WAIT_TIMEOUT;
		}

	private:
		PROCESS_INFORMATION m_pi;
		bool m_hasClosed = false;
		bool m_valid = false;

		friend ScopedPtr<ChildProcess> create_child_process(const std::string&);
	};

	ScopedPtr<ChildProcess> create_child_process(const std::string& args)
	{
		if (args.empty())
			return daedalus::ScopedPtr<WindowsChildProcess>();

		// This seems weird but CreateProcess needs a char* which crashes if I
		// remove the const from args, so I just make a copy of the string
		char* nonConstString = new char[strlen(args.c_str()) + 1];
		strcpy(nonConstString, args.c_str());

		STARTUPINFOA si;

		// I didn't really want to make it on the heap however i couldnt find a better way,
		// while still being able to abstract as the windows specific parts away
		WindowsChildProcess* newProcess = new WindowsChildProcess();

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&newProcess->m_pi, sizeof(newProcess->m_pi));

		if (!CreateProcessA(
			NULL,	// No module name (use command line)
			nonConstString,	// Command line
			NULL,	// Process handle not inheritable
			NULL,	// Thread handle not inheritable
			FALSE,	// Set handle inheritance to False
			0,		// No Creation flags
			NULL,	// Use parent's environment block
			NULL,	// Use parent's starting directory
			&si,	// Pointer to STARTUPINFO structure
			&newProcess->m_pi))	// Pointer to PROCESS_INFORMATION structure
		{
			newProcess->m_valid = false;
			DD_LOG_ERROR("create_child_process: Failed to create process with args {}", args);
		}
		else
		{
			newProcess->m_valid = true;
		}

		return daedalus::ScopedPtr<WindowsChildProcess>(newProcess);
	}

}