local DaedalusRootDir = os.getenv("DAEDALUS_DIR")

workspace (_ARGS[2])
	architecture "x64"
	location (_ARGS[1])
	startproject "Scripting"

	configurations
	{
		"Debug",
		"Release",
		"Distro"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "CSharp-Scripting"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	location (_ARGS[1])

	targetdir (_ARGS[1] .. "/script-bin")
	objdir (_ARGS[1] .. "/script-bin/intermediates")

	files
	{
		_ARGS[1] .. "/assets/**.cs",
	}

	links
	{
		"Daedalus-ScriptCore"
	}

	filter "configurations:Debug"
		optimize "off"
		symbols "default"

	filter "configurations:Realease"
		optimize "on"
		symbols "default"

	filter "configurations:Distro"
		optimize "full"
		symbols "off"

group "Daedalus"
	include (DaedalusRootDir .. "/Daedalus-ScriptCore")
group ""