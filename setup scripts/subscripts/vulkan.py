import os
import subprocess
import sys
from pathlib import Path

import subscripts.utils

from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_INSTALLER_URL = 'https://sdk.lunarg.com/sdk/download/1.4.321.1/windows/vulkan_sdk.exe'
DD_VULKAN_VERSION = '1.4.321.1'
VULKAN_SDK_EXE_PATH = 'Daedalus-Core/Dependencies/VulkanSDK/VulkanSDK.exe'

def installVulkanSDK():
    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))
    subscripts.utils.downloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Done!")
    print("Running Vulkan SDK installer...")
    print("Make sure to install 'Shader Toolchain Debug Symbols - 64-bit' as a component, when prompted")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Re-run this script after installation")

def installVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = subscripts.utils.yesOrNo()
    if (install):
        InstallVulkanSDK()
        quit()

def checkVulkanSDK():
    if (VULKAN_SDK is None):
        print("You don't have the Vulkan SDK installed!")
        installVulkanPrompt()
        return False
    elif (DD_VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"You don't have the correct Vulkan SDK version! (Daedalus requires {DD_VULKAN_VERSION})")
        installVulkanPrompt()
        return False
    
    print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
    return True

VulkanSDKDebugLibsURL = 'https://files.lunarg.com/SDK-1.4.321.1/VulkanSDK-1.4.321.1-DebugLibs.zip'
OutputDirectory = "Daedalus-Core/Dependencies/VulkanSDK"
TempZipFile = f"{OutputDirectory}/VulkanSDK.zip"

def checkVulkanSDKDebugLibs():
    shadercdLib = Path(f"{OutputDirectory}/Lib/shaderc_sharedd.lib")
    if (not shadercdLib.exists()):
        print(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
        print("Downloading", VulkanSDKDebugLibsURL)
        with urlopen(VulkanSDKDebugLibsURL) as zipresp:
            with ZipFile(BytesIO(zipresp.read())) as zfile:
                zfile.extractall(OutputDirectory)

    print(f"Vulkan SDK debug libs located at {OutputDirectory}")
    return True