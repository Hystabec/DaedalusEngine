include "dependencies.lua"

workspace "DaedalusEngine"
	architecture "x64"
	startproject "Daedalus-Editor"

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

group "Dependecies"
	include "Daedalus-Core/dependencies/ImGui"
	include "Daedalus-Core/dependencies/yaml-cpp"
	include "Daedalus-Core/dependencies/Box2D"
	include "Daedalus-Core/dependencies/msdf-atlas-gen"
group ""

group "Core"
include "Daedalus-Core"
include "Daedalus-ScriptCore"
group ""

group "Tools"
include "Daedalus-Editor"
group ""

group "Misc"
include "Sandbox"
group ""