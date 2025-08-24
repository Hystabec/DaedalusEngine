local DaedalusRootDir = "../../../.."

workspace "Sandbox"
	architecture "x64"
	startproject "Sandbox"

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

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("script-bin")
	objdir ("script-bin/intermediates")

	files
	{
		"src/**.cs",
		"properties/**.cs"
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