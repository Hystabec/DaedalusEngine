@echo off
pushd %~dp0\..\
echo Make sure to run 'setup.bat' to install necessary vulkan libraries (If not already done)
echo Running premake...
call Dependencies\premake\premake5.exe vs2022
popd
pause