workspace "DaedalusEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Realease",
		"Distro"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
dependdir = "%{prj.name}/Dependencies"

project "Daedalus-Core"
	location "Daedalus-Core"
	kind "SharedLib"
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
		"%{prj.name}/src",
		dependdir .. "GLEW/include",
		dependdir .. "GLFW/include",
		dependdir .. "FreeImage/include",
		dependdir .. "spdlog/include"
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