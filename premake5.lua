workspace "DaedalusEngine"
	architecture "x64"
	startproject "Daedalus-Editor"

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
dependDir["spdlog"] = dependFol .. "/spdlog"
dependDir["ImGui"] = dependFol .. "/ImGui"
dependDir["stb_image"] = dependFol .. "/stb_image"
dependDir["entt"] = dependFol .. "/Entt"

group "Dependecies"
	include "Daedalus-Core/Dependencies/ImGui"
group ""

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
		"%{prj.name}/entryPoint.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Dependencies/stb_image/**.h",
		"%{prj.name}/Dependencies/stb_image/**.cpp"
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
		"%{dependDir.spdlog}/include",
		"%{dependDir.ImGui}/include",
		"%{dependDir.stb_image}",
		"%{dependDir.entt}/include"
	}

	libdirs
	{
		"%{dependDir.GLFW}/lib-vc2022",
		"%{dependDir.GLEW}/lib"
	}

	links
	{
		"ImGui",
		"glfw3_mt.lib",
		"opengl32.lib",
		"glew32s.lib"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS",
			"DD_RENDER_USING_OPENGL",
			"GLEW_STATIC"
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
		--"%{dependDir.entt}/include"
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


project "Daedalus-Editor"
	location "Daedalus-Editor"
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
		--"%{dependDir.entt}/include"
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