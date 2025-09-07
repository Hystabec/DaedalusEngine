#pragma once

// This is based on the file watcher by ThomasMonkman (https://github.com/ThomasMonkman/filewatch)
// I have rewitten parts of, and modified it to work for my use case and follow my conventions

#include "../debugTools/logging/log.h"

// WIN32
#include <windows.h>
// -WIN32

#include <filesystem>
#include <functional>
#include <vector>
#include <array>
#include <regex>

#include <thread>
#include <mutex>
#include <future>

namespace daedalus::utils {

	class FileWatcher
	{
	public:
		enum class Event
		{
			Added,
			Removed,
			Modified,
			RenamedOld,
			RenamedNew,
		};

		FileWatcher() = default;

		FileWatcher(const std::filesystem::path& path, const std::function<void(const std::filesystem::path&, Event)>& callback)
			: m_path(path), m_callback(callback), m_watchingDirectory(path.filename().empty()), m_directory(getDirectoryHandle(path))
		{
			init();
		}

		FileWatcher(FileWatcher&&) = delete;

		FileWatcher& operator=(const FileWatcher& other)
		{
			if (this == &other)
				return *this;

			destroy();
			m_path = other.m_path;
			m_callback = other.m_callback;
			m_watchingDirectory = other.m_watchingDirectory;
			m_directory = getDirectoryHandle(m_path);

			m_destory = false;

			init();

			return *this;
		}

		~FileWatcher()
		{
			destroy();
		}

	private:
		
		void init()
		{
			initCalled = true;

			if (!m_directory)
			{
				DD_CORE_LOG_ERROR("Failed to create directory handle");
				return;
			}

			m_closeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (!m_closeEvent)
			{
				DD_CORE_LOG_ERROR("FileWatcher: Could not create close event");
				return;
			}

			m_callbackThread = std::thread([this]()
				{
					try
					{
						callbackThread();
					}
					catch (...)
					{
						m_running.set_exception(std::current_exception());
					}
				});

			m_watchThread = std::thread([this]()
				{
					try
					{
						monitorDirectory();
					}
					catch (...)
					{
						m_running.set_exception(std::current_exception());
					}
				});

			// WIN32 ?
			SetThreadDescription(m_callbackThread.native_handle(), L"FileWatcher-Callback");
			SetThreadDescription(m_watchThread.native_handle(), L"FileWatcher-Watch");
			// -WIN32

			std::future<void> future = m_running.get_future();
			future.get(); // block until monitorDirectory is going
		}

		void destroy()
		{
			if (!initCalled)
				return;

			m_destory = true;
			m_running = std::promise<void>();

			SetEvent(m_closeEvent);

			m_cv.notify_all();

			if(m_watchThread.joinable())
				m_watchThread.join();

			if(m_callbackThread.joinable())
				m_callbackThread.join();

			// WIN32
			CloseHandle(m_directory);
			// -WIN32
		}

		void callbackThread()
		{
			while (m_destory == false)
			{
				std::unique_lock<std::mutex> lock(m_callbackMutex);

				if (m_callbackInfo.empty() && m_destory == false)
				{
					m_cv.wait(lock, [this] {return m_callbackInfo.size() > 0 || m_destory; });
				}

				std::vector<std::pair<std::filesystem::path, Event>> callbackInfo = {};
				std::swap(callbackInfo, m_callbackInfo);
				lock.unlock();

				for (const auto& file : callbackInfo)
				{
					if (m_callback)
					{
						try
						{
							m_callback(file.first, file.second);
						}
						catch (const std::exception& e)
						{
							DD_CORE_LOG_ERROR("fileWatcher: {}", e.what());
						}
					}
				}
			}
		}

		void monitorDirectory()
		{
			std::vector<byte> buffer(m_bufferSize);
			DWORD bytesReturned = 0;
			OVERLAPPED overlappedBuffer{ 0 };

			overlappedBuffer.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (!overlappedBuffer.hEvent)
				DD_CORE_LOG_ERROR("Error creating monitor event");

			std::array<HANDLE, 2> handles{ overlappedBuffer.hEvent, m_closeEvent };
			auto asyncPending = false;
			m_running.set_value();
			while (m_destory == false)
			{
				constexpr DWORD filters =
					FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_ACCESS |
					FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES |
					FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME;

				std::vector<std::pair<std::filesystem::path, Event>> parsedInfo;
				ReadDirectoryChangesW(m_directory,
					buffer.data(), static_cast<DWORD>(buffer.size()),
					TRUE,
					filters,
					&bytesReturned,
					&overlappedBuffer,
					NULL);

				asyncPending = true;

				switch (WaitForMultipleObjects(2, handles.data(), FALSE, INFINITE))
				{
				case WAIT_OBJECT_0:
				{
					if (!GetOverlappedResult(m_directory, &overlappedBuffer, &bytesReturned, TRUE))
						throw std::system_error(GetLastError(), std::system_category());

					asyncPending = false;

					if (bytesReturned == 0)
						break;

					FILE_NOTIFY_INFORMATION* fileInfomation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);
					while (true)
					{
						std::filesystem::path changedFilePath{ std::wstring{fileInfomation->FileName, fileInfomation->FileNameLength / sizeof(fileInfomation->FileName[0])} };
						if (passFilter(changedFilePath))
							parsedInfo.emplace_back(changedFilePath, m_eventTypeMapping.at(fileInfomation->Action));

						if (fileInfomation->NextEntryOffset == 0)
							break;

						fileInfomation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<byte*>(fileInfomation) + fileInfomation->NextEntryOffset);
					}

					break;
				}
				case WAIT_OBJECT_0 + 1:
					break; // quit
				case WAIT_FAILED:
					break;
				}
				{
					std::lock_guard<std::mutex> lock(m_callbackMutex);
					m_callbackInfo.insert(m_callbackInfo.end(), parsedInfo.begin(), parsedInfo.end());
				}
				m_cv.notify_all();
			};

			if (asyncPending)
			{
				CancelIo(m_directory);
				GetOverlappedResult(m_directory, &overlappedBuffer, &bytesReturned, TRUE);
			}
		}

		bool passFilter(const std::filesystem::path& path)
		{
			if (!m_watchingDirectory)
			{
				return path == m_path.filename();
			}
			constexpr std::string::value_type regexExpr[] = { '.', '*', '\0' };
			const static std::basic_regex<std::string::value_type, std::regex_traits<std::string::value_type>> regex(regexExpr);
			return std::regex_match(path.string(), regex);
		}

		HANDLE getDirectoryHandle(const std::filesystem::path& path)
		{
			auto fileInfo = GetFileAttributes(path.c_str());

			if (fileInfo == INVALID_FILE_ATTRIBUTES)
				throw std::system_error(GetLastError(), std::system_category());

			m_watchingDirectory = (fileInfo & FILE_ATTRIBUTE_DIRECTORY) != 0;

			std::filesystem::path withoutFile = path;
			withoutFile.remove_filename();
			std::filesystem::path toFile = m_watchingDirectory ? path : withoutFile.empty() ? ".\\" : withoutFile;

			HANDLE directory = CreateFile(toFile.c_str(),
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				nullptr,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
				HANDLE(0));

			if (directory == INVALID_HANDLE_VALUE)
				throw std::system_error(GetLastError(), std::system_category());
			 
			return directory;
		}

	private:
		std::filesystem::path m_path;
		std::function<void(const std::filesystem::path&, Event)> m_callback;
		bool m_watchingDirectory = false;
		bool initCalled = false;

		std::atomic_bool m_destory = false;
		std::promise<void> m_running;

		std::condition_variable m_cv;

		std::thread m_callbackThread;
		std::mutex m_callbackMutex;
		std::thread m_watchThread;

		std::vector<std::pair<std::filesystem::path, Event>> m_callbackInfo;

		static constexpr std::size_t m_bufferSize = { 1024 * 256 };

		// WIN32
		HANDLE m_closeEvent = nullptr;
		HANDLE m_directory = nullptr;

		const std::unordered_map<DWORD, Event> m_eventTypeMapping = {
			{ FILE_ACTION_ADDED,			Event::Added		},
			{ FILE_ACTION_REMOVED,			Event::Removed		},
			{ FILE_ACTION_MODIFIED,			Event::Modified		},
			{ FILE_ACTION_RENAMED_OLD_NAME, Event::RenamedOld	},
			{ FILE_ACTION_RENAMED_NEW_NAME, Event::RenamedNew	}
		};
		// -WIN32
	};

}