project "Daedalus-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "editorpch.h"
	pchsource "src/editorpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/**.hpp"
	}

	includedirs
	{
		"src",
		"../Daedalus-Core",
		"../Daedalus-Core/src",
		"%{dependDir.ImGui}/include",
		"%{dependDir.entt}/include"
	}

	links
	{
		"Daedalus-Core"
	}

	postbuildcommands
	{
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"DD_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines 
		{
			"DD_DEBUG",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		}
		symbols "on"

	filter "configurations:Realease"
		defines
		{
			"DD_RELEASE",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		} 
		optimize "on"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "on"