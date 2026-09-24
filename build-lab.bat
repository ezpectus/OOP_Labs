@echo off
rem build-lab.bat N [SubProgram] [run]
rem   Auto-builds lab N: scans labs\labN\code\src for *.cpp and ..\res for *.rc,
rem   compiles everything, links LabN.exe. Works from any folder.
rem   "run" as last arg launches the exe after build.
rem   For multi-program labs (lab6): build-lab.bat 6 Object2 [run].
if "%1"=="" (
  echo Usage: build-lab.bat ^<lab number^> [program subfolder] [run]
  echo   build-lab.bat 2          builds labs\lab2
  echo   build-lab.bat 3 run      builds labs\lab3 and launches it
  echo   build-lab.bat 6 Object2  builds labs\lab6\code\src\Object2
  exit /b 1
)

set RUN=0
set SUB=
if /i "%2"=="run" (set RUN=1) else (set SUB=%2)
if /i "%3"=="run" set RUN=1

set SRCDIR=labs\lab%1\code\src
if not "%SUB%"=="" set SRCDIR=%SRCDIR%\%SUB%

if not exist "%~dp0%SRCDIR%" (
  echo ERROR: folder %SRCDIR% not found
  exit /b 1
)

cd /d "%~dp0%SRCDIR%"
echo Building in %CD%

for %%f in (..\res\*.rc) do (
  echo   windres %%~nxf
  windres -c 65001 --output-format=coff -I . -i "%%f" -o "%%~nf.res.o" || exit /b 1
)
if "%SUB%" neq "" for %%f in (*.rc) do (
  echo   windres %%~nxf
  windres -c 65001 --output-format=coff -I . -i "%%f" -o "%%~nf.res.o" || exit /b 1
)

for %%f in (*.cpp) do (
  echo   g++ %%~nxf
  g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c "%%f" -o "%%~nf.o" || exit /b 1
)

set EXE=Lab%1
if not "%SUB%"=="" set EXE=%SUB%
rem close running instance so the link step can overwrite the exe
taskkill /f /im %EXE%.exe >nul 2>&1
g++ -municode -mwindows -o %EXE%.exe *.o -lgdi32 -luser32 -lcomctl32 -lcomdlg32 || exit /b 1

echo.
echo === %EXE%.exe built ===

if "%RUN%"=="1" start "" "%EXE%.exe"
