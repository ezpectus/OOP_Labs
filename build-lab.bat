@echo off
rem build-lab.bat N [SubProgram]
rem   Auto-builds lab N: scans labs\labN\code\src for *.cpp and ..\res for *.rc,
rem   compiles everything, links LabN.exe. Works from any folder.
rem   For multi-program labs (lab6): build-lab.bat 6 Object2 builds that program.
if "%1"=="" (
  echo Usage: build-lab.bat ^<lab number^> [program subfolder]
  echo   build-lab.bat 2          builds labs\lab2
  echo   build-lab.bat 6 Object2  builds labs\lab6\code\src\Object2
  exit /b 1
)

set SRCDIR=labs\lab%1\code\src
if not "%2"=="" set SRCDIR=%SRCDIR%\%2

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
if "%2" neq "" for %%f in (*.rc) do (
  echo   windres %%~nxf
  windres -c 65001 --output-format=coff -I . -i "%%f" -o "%%~nf.res.o" || exit /b 1
)

for %%f in (*.cpp) do (
  echo   g++ %%~nxf
  g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c "%%f" -o "%%~nf.o" || exit /b 1
)

set EXE=Lab%1
if not "%2"=="" set EXE=%2
g++ -municode -mwindows -o %EXE%.exe *.o -lgdi32 -luser32 -lcomctl32 -lcomdlg32 || exit /b 1

echo.
echo === %EXE%.exe built ===
