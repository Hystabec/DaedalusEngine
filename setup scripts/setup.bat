@echo off
python --version 3>NUL
if errorlevel 1 goto :errorNoPython

echo Python 3 is intalled
echo starting setup
python setup.py
goto :finalPause

:errorNoPython
echo.
echo Python3 is not installed on your system.
echo Please install Python 3 or newer : 'https://www.python.org/downloads/'
goto :finalPause

:finalPause
pause