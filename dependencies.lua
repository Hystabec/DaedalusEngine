-- Include dirs relative to root
VULKAN_SDK = os.getenv("VULKAN_SDK")

dependDir = {}
dependDir["GLFW"] =         "%{wks.location}/Daedalus-Core/dependencies/GLFW"
dependDir["GLEW"] =         "%{wks.location}/Daedalus-Core/dependencies/GLEW"
dependDir["spdlog"] =       "%{wks.location}/Daedalus-Core/dependencies/spdlog"
dependDir["ImGui"] =        "%{wks.location}/Daedalus-Core/dependencies/ImGui"
dependDir["stb_image"] =    "%{wks.location}/Daedalus-Core/dependencies/stb_image"
dependDir["entt"] =         "%{wks.location}/Daedalus-Core/dependencies/Entt"
dependDir["yaml_cpp"] =     "%{wks.location}/Daedalus-Core/dependencies/yaml-cpp"
dependDir["ImGuizmo"] =     "%{wks.location}/Daedalus-Core/dependencies/ImGuizmo"
dependDir["Box2D"] =        "%{wks.location}/Daedalus-Core/dependencies/Box2D"
dependDir["mono"] =         "%{wks.location}/Daedalus-Core/dependencies/mono"
dependDir["VulkanSDK"] =    "%{VULKAN_SDK}"

libraryDir = {}
libraryDir["mono"] =        "%{dependDir.mono}/lib/%{cfg.buildcfg}"
libraryDir["VulkanSDK"] =   "%{VULKAN_SDK}/Lib"

library = {}
library["mono"] =                       "%{libraryDir.mono}/libmono-static-sgen.lib"

library["ShaderC_Debug"] =              "%{libraryDir.VulkanSDK}/shaderc_sharedd.lib"
library["SPIRV_Cross_Debug"] =          "%{libraryDir.VulkanSDK}/spirv-cross-cored.lib"
library["SPIRV_Cross_GLSL_Debug"] =     "%{libraryDir.VulkanSDK}/spirv-cross-glsld.lib"
library["SPIRV_Tools_Debug"] =          "%{libraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

library["ShaderC_Release"] =            "%{libraryDir.VulkanSDK}/shaderc_shared.lib"
library["SPIRV_Cross_Release"] =        "%{libraryDir.VulkanSDK}/spirv-cross-core.lib"
library["SPIRV_Cross_GLSL_Release"] =   "%{libraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
library["winSock"] =    "Ws2_32.lib"
library["winMM"] =      "Winmm.lib"
library["winVersion"] = "Version.lib"
library["winBCypt"] =   "Bcrypt.lib"