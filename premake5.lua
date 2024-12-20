workspace "DaedalusEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Realease",
		"Distro"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

dependDir = {}
dependDir["GLFW"] = "Daedalus-Core/Dependencies/GLFW/include"
dependDir["GLEW"] = "Daedalus-Core/Dependencies/GLEW/include"
dependDir["FreeImage"] = "Daedalus-Core/Dependencies/FreeImage/include"
dependDir["spdlog"] = "Daedalus-Core/Dependencies/spdlog/include"

project "Daedalus-Core"
	location "Daedalus-Core"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "ddpch.h"
	pchsource "Daedalus-Core/src/ddpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{dependDir.GLFW}",
		"%{dependDir.GLEW}",
		"%{dependDir.FreeImage}",
		"%{dependDir.spdlog}"
	}

	links
	{
		"glfw3.lib",
		"opengl32.lib",
		"FreeImage.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS",
			"DD_BUILD_DLL"
		}

		postbuildcommands
		{
			("{copy} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "DD_DEBUG"
		symbols "On"

	filter "configurations:Realease"
		defines "DD_RELEASE"
		optimize "On"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"Daedalus-Core"
	}

	links
	{
		"Daedalus-Core"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DD_DEBUG"
		symbols "On"

	filter "configurations:Realease"
		defines "DD_RELEASE"
		optimize "On"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "On"