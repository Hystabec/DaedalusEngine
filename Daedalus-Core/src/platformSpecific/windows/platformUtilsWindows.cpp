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

}