project "Daedalus-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"Dependencies/stb_image/**.cpp",
		"Dependencies/ImGuizmo/ImGuizmo.h",
		"Dependencies/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
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
		"%{dependDir.yaml_cpp}/include",
		"%{dependDir.ImGuizmo}",
		"%{dependDir.Box2D}/include",
		"%{dependDir.VulkanSDK}/Include"
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
		"yaml-cpp",
		"Box2D"
	}

	buildoptions "/utf-8"

	filter "files:Dependencies/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DD_PLATFORM_WINDOWS",
			"DD_RENDER_USING_OPENGL",
			"GLEW_STATIC"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines
		{
			"DD_DEBUG",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		}
		
		links
		{
			"%{library.ShaderC_Debug}",
			"%{library.SPIRV_Cross_Debug}",
			"%{library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		defines
		{
			"DD_RELEASE",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		}

		links
		{
			"%{library.ShaderC_Release}",
			"%{library.SPIRV_Cross_Release}",
			"%{library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Distro"
		runtime "Release"
		optimize "on"
		defines 
		{
			"DD_DISTRO"
		}

		links
		{
			"%{library.ShaderC_Release}",
			"%{library.SPIRV_Cross_Release}",
			"%{library.SPIRV_Cross_GLSL_Release}"
		}