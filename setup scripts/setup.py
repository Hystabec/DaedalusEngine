import os
import subprocess
import subscripts.checkPython

subscripts.checkPython.validatePackages()

import subscripts.vulkan

os.chdir('../')

if (not subscripts.vulkan.checkVulkanSDK()):
    print("Vulkan SDK debug libs not found.")

if (not subscripts.vulkan.checkVulkanSDKDebugLibs()):
    print("Vulkan SDK debug libs not found.")

print("Running premake...")
subprocess.call(["Dependencies\\premake\\premake5.exe", "vs2022"])