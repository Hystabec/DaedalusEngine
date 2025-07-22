project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/**.hpp"
	}

	-- Dont think i should need to define YAML static as it not used in this project
	-- however it doesnt seem to compile if it dont
	-- TO DO: investigate if YAML is being used anywhere / accidnetly included in a header file etc.
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
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