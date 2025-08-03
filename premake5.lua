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
	include "Daedalus-Core/Dependencies/ImGui"
	include "Daedalus-Core/Dependencies/yaml-cpp"
	include "Daedalus-Core/Dependencies/Box2D"
group ""

include "Daedalus-Core"
include "Daedalus-Editor"
include "Sandbox"