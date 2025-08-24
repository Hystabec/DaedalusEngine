project "Daedalus-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../Daedalus-Core/resources/script-bin")
	objdir ("../Daedalus-Core/resources/script-bin/intermediates")

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