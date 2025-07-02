project "Daedalus-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "ddpch.h"
	pchsource "src/ddpch.cpp"

	files
	{
		"Daedalus.h",
		"entryPoint.h",
		"src/**.h",
		"src/**.cpp",
		"Dependencies/stb_image/**.h",
		"Dependencies/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{dependDir.GLFW}/include",
		"%{dependDir.GLEW}/include",
		"%{dependDir.spdlog}/include",
		"%{dependDir.ImGui}/include",
		"%{dependDir.stb_image}",
		"%{dependDir.entt}/include",
		"%{dependDir.yaml_cpp}"
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
		"glew32s.lib",
		"yaml-cpp"
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