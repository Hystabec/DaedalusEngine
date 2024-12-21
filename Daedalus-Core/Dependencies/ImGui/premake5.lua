project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/imconfig.h",
		"include/imgui.h",
		"include/imgui.cpp",
		"include/imgui_draw.cpp",
		"include/imgui_internal.h",
		"include/imgui_widgets.cpp",
		"include/imstb_rectpack.h",
		"include/imstb_textedit.h",
		"include/imstb_truetype.h",
		"include/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
