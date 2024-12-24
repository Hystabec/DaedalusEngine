workspace "DaedalusEngine"
	architecture "x64"
	startproject "Sandbox"

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
dependDir["ImGui"] = dependFol .. "/ImGui"

include "Daedalus-Core/Dependencies/ImGui"

project "Daedalus-Core"
	location "Daedalus-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

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

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{dependDir.GLFW}/include",
		"%{dependDir.GLEW}/include",
		"%{dependDir.FreeImage}/include",
		"%{dependDir.spdlog}/include",
		"%{dependDir.ImGui}/include"
	}

	libdirs
	{
		"%{dependDir.GLFW}/lib-vc2022",
		"%{dependDir.GLEW}/lib",
		"%{dependDir.FreeImage}/lib",
	}

	links
	{
		"ImGui",
		"glfw3_mt.lib",
		"opengl32.lib",
		"glew32s.lib",
		"FreeImage.lib"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS",
			"GLEW_STATIC"
		}

	filter "configurations:Debug"
		defines
		{
			"DD_DEBUG",
			"DD_USING_ASSERTS"
		}
		symbols "on"

	filter "configurations:Realease"
		defines
		{
			"DD_RELEASE",
			"DD_USING_ASSERTS"
		}
		optimize "on"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

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
		"Daedalus-Core",
		"Daedalus-Core/src",
		"%{dependDir.ImGui}/include"
	}

	links
	{
		"Daedalus-Core"
	}

	postbuildcommands
	{
		("{copy} ../Daedalus-Core/Dependencies/FreeImage/FreeImage.dll ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DD_DEBUG"
		symbols "on"

	filter "configurations:Realease"
		defines "DD_RELEASE"
		optimize "on"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "on"