@echo off
rem build.bat - builds ALL labs via build-lab.bat (auto-scans src/*.cpp + res/*.rc)
echo === Building labs with MinGW (g++) ===

call "%~dp0build-lab.bat" 1 || exit /b 1
call "%~dp0build-lab.bat" 2 || exit /b 1
call "%~dp0build-lab.bat" 3 || exit /b 1

echo.
echo === All labs built ===
