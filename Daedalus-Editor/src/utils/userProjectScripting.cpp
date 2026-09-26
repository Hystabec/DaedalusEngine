#include "editorpch.h"
#include "userProjectScripting.h"

#include "utils/platformUtils.h"

bool daedalus::editor::generate_user_scripting_project()
{
	// NOTE / TODO: if the project name is ever changed the scriping files might want to be regenerated
	// other wise they will still use the old name (However it should still work so might not be needed)

	// TODO: instead of checking if it is in distro check if it has been built
	// same applies to findFileLocation.h
#ifndef DD_DISTRO
	std::filesystem::path premakePath = "..\\dependencies\\premake\\";
#else
	std::filesystem::path premakePath = "vendor\\premake\\";
#endif
	bool timedOut = false;
	{
		// NOTE: USDPremake.lua = U(user)S(scripting)D(default)Premake.lua
		std::string premakeArgs = std::format("\"{0}premake5.exe\" --file=\"{0}USDPremake.lua\" vs2022 \"{1}\" \"{2}\"",
			premakePath.string(), Project::getActiveProjectDirectory().string(), Project::getActive()->getConfig().name);

		// TODO: Running this causes any premake (and by extension any program) output to be dumped into the console
		// which looks a little weird, add a way to supress these / launch their own console
		ScopedPtr<utils::ChildProcess> premakeProcess = utils::create_child_process(premakeArgs);
		if (!premakeProcess->waitForProcess(5000))
		{
			DD_LOG_ERROR("Premake process timed out");
			timedOut = true;
		}
	}

	return !timedOut && std::filesystem::exists(Project::getActiveProjectDirectory() / "UserScripting.sln");
}