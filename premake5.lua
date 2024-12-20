workspace "DaedalusEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distro"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

dependFol = "Daedalus-Core/Dependencies"

dependDir = {}
dependDir["GLFW"] = dependFol .. "/GLFW"
dependDir["GLEW"] = dependFol .. "/GLEW"
dependDir["FreeImage"] = dependFol .. "/FreeImage"
dependDir["spdlog"] = dependFol .. "/spdlog"

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
		"%{prj.name}/Daedalus.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{dependDir.GLFW}/include",
		"%{dependDir.GLEW}/include",
		"%{dependDir.FreeImage}/include",
		"%{dependDir.spdlog}/include"
	}

	libdirs
	{
		"%{dependDir.GLFW}/lib-vc2022",
		"%{dependDir.GLEW}/lib",
		"%{dependDir.FreeImage}/lib",
	}

	links
	{
		"glfw3.lib",
		"glew32s.lib",
		"opengl32.lib",
		"FreeImage.lib"
	}

	buildoptions "/utf-8"

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