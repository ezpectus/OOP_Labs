@echo off
echo === Building labs with MinGW (g++) ===

set CXXFLAGS=-std=c++26 -DUNICODE -D_UNICODE -municode -O2
set LDFLAGS=-lgdi32 -luser32 -lcomctl32

echo.
echo --- Lab 1 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab1\code\src"
windres --output-format=coff -I . -i ..\res\Lab1.rc -o Lab1.res.o
windres --output-format=coff -I . -i ..\res\module1.rc -o module1.res.o
g++ %CXXFLAGS% -c Lab1.cpp -o Lab1.o
g++ %CXXFLAGS% -c module1.cpp -o module1.o
g++ -municode -o Lab1.exe Lab1.o module1.o Lab1.res.o module1.res.o %LDFLAGS%
echo Lab1.exe built.

echo.
echo === Done ===
