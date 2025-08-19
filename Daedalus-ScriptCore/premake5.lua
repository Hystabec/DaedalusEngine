project "Daedalus-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Daedalus-Core/resources/scripts")
	objdir ("%{wks.location}/Daedalus-Core/resources/scripts/intermediates")

	files
	{
		"src/**.cs",
		"properties/**.cs"
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