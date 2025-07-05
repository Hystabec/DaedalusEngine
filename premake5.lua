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

-- Include dirs relative to root
dependDir = {}
dependDir["GLFW"] =			"%{wks.location}/Daedalus-Core/Dependencies/GLFW"
dependDir["GLEW"] =			"%{wks.location}/Daedalus-Core/Dependencies/GLEW"
dependDir["spdlog"] =		"%{wks.location}/Daedalus-Core/Dependencies/spdlog"
dependDir["ImGui"] =		"%{wks.location}/Daedalus-Core/Dependencies/ImGui"
dependDir["stb_image"] =	"%{wks.location}/Daedalus-Core/Dependencies/stb_image"
dependDir["entt"] =			"%{wks.location}/Daedalus-Core/Dependencies/Entt"
dependDir["yaml_cpp"] =		"%{wks.location}/Daedalus-Core/Dependencies/yaml-cpp"
dependDir["ImGuizmo"] =		"%{wks.location}/Daedalus-Core/Dependencies/ImGuizmo"

group "Dependecies"
	include "Daedalus-Core/Dependencies/ImGui"
	include "Daedalus-Core/Dependencies/yaml-cpp"
group ""

include "Daedalus-Core"
include "Daedalus-Editor"
include "Sandbox"