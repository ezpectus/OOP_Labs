@echo off
echo === Building all labs with MinGW (g++) ===

set CXXFLAGS=-std=c++26 -DUNICODE -D_UNICODE -municode -O2
set LDFLAGS=-lgdi32 -luser32 -lcomctl32

echo.
echo --- Lab 1 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab1\code\src"
windres --output-format=coff -I . -i ..\res\Lab1.rc -o Lab1.res.o
windres --output-format=coff -I . -i ..\res\module1.rc -o module1.res.o
windres --output-format=coff -I . -i ..\res\module2.rc -o module2.res.o
g++ %CXXFLAGS% -c Lab1.cpp -o Lab1.o
g++ %CXXFLAGS% -c module1.cpp -o module1.o
g++ %CXXFLAGS% -c module2.cpp -o module2.o
g++ -municode -o Lab1.exe Lab1.o module1.o module2.o Lab1.res.o module1.res.o module2.res.o %LDFLAGS%
echo Lab1.exe built.

echo.
echo --- Lab 2 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab2\code\src"
windres --output-format=coff -I . -i ..\res\Lab2.rc -o Lab2.res.o
g++ %CXXFLAGS% -c Lab2.cpp -o Lab2.o
g++ %CXXFLAGS% -c shape.cpp -o shape.o
g++ %CXXFLAGS% -c point.cpp -o point.o
g++ %CXXFLAGS% -c line.cpp -o line.o
g++ %CXXFLAGS% -c rect.cpp -o rect.o
g++ %CXXFLAGS% -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab2.exe Lab2.o shape.o point.o line.o rect.o ellipse.o Lab2.res.o %LDFLAGS%
echo Lab2.exe built.

echo.
echo --- Lab 3 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab3\code\src"
windres --output-format=coff -I . -i ..\res\Lab3.rc -o Lab3.res.o
g++ %CXXFLAGS% -c Lab3.cpp -o Lab3.o
g++ %CXXFLAGS% -c shape.cpp -o shape.o
g++ %CXXFLAGS% -c point.cpp -o point.o
g++ %CXXFLAGS% -c line.cpp -o line.o
g++ %CXXFLAGS% -c rect.cpp -o rect.o
g++ %CXXFLAGS% -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab3.exe Lab3.o shape.o point.o line.o rect.o ellipse.o Lab3.res.o %LDFLAGS%
echo Lab3.exe built.

echo.
echo --- Lab 4 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab4\code\src"
windres --output-format=coff -I . -i ..\res\Lab4.rc -o Lab4.res.o
g++ %CXXFLAGS% -c Lab4.cpp -o Lab4.o
g++ %CXXFLAGS% -c my_editor.cpp -o my_editor.o
g++ %CXXFLAGS% -c shape.cpp -o shape.o
g++ %CXXFLAGS% -c point.cpp -o point.o
g++ %CXXFLAGS% -c line.cpp -o line.o
g++ %CXXFLAGS% -c rect.cpp -o rect.o
g++ %CXXFLAGS% -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab4.exe Lab4.o my_editor.o shape.o point.o line.o rect.o ellipse.o Lab4.res.o %LDFLAGS%
echo Lab4.exe built.

echo.
echo --- Lab 5 ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab5\code\src"
windres --output-format=coff -I . -i ..\res\Lab5.rc -o Lab5.res.o
windres --output-format=coff -I . -i ..\res\my_table.rc -o my_table.res.o
g++ %CXXFLAGS% -c Lab5.cpp -o Lab5.o
g++ %CXXFLAGS% -c my_editor.cpp -o my_editor.o
g++ %CXXFLAGS% -c my_table.cpp -o my_table.o
g++ %CXXFLAGS% -c shape.cpp -o shape.o
g++ %CXXFLAGS% -c point.cpp -o point.o
g++ %CXXFLAGS% -c line.cpp -o line.o
g++ %CXXFLAGS% -c rect.cpp -o rect.o
g++ %CXXFLAGS% -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab5.exe Lab5.o my_editor.o my_table.o shape.o point.o line.o rect.o ellipse.o Lab5.res.o my_table.res.o %LDFLAGS%
echo Lab5.exe built.

echo.
echo --- Lab 6 (3 programs) ---
cd /d "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src"
windres --output-format=coff -I . -i ..\res\Lab6.rc -o Lab6.res.o
g++ %CXXFLAGS% -c Lab6.cpp -o Lab6.o
g++ -municode -o Lab6.exe Lab6.o Lab6.res.o %LDFLAGS%
echo Lab6.exe built.

cd /d "F:\VSC projects\OOP_Labs\labs\lab6\code\Object2\src"
g++ %CXXFLAGS% -c Object2.cpp -o Object2.o
g++ -municode -o Object2.exe Object2.o -lgdi32 -luser32
echo Object2.exe built.

cd /d "F:\VSC projects\OOP_Labs\labs\lab6\code\Object3\src"
g++ %CXXFLAGS% -c Object3.cpp -o Object3.o
g++ -municode -o Object3.exe Object3.o -lgdi32 -luser32
echo Object3.exe built.

echo.
echo Copying Object2.exe and Object3.exe next to Lab6.exe...
copy /Y "F:\VSC projects\OOP_Labs\labs\lab6\code\Object2\src\Object2.exe" "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src\Object2.exe"
copy /Y "F:\VSC projects\OOP_Labs\labs\lab6\code\Object3\src\Object3.exe" "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src\Object3.exe"

echo.
echo === All labs built successfully! ===
echo.
echo Run commands:
echo   Lab1: labs\lab1\code\src\Lab1.exe
echo   Lab2: labs\lab2\code\src\Lab2.exe
echo   Lab3: labs\lab3\code\src\Lab3.exe
echo   Lab4: labs\lab4\code\src\Lab4.exe
echo   Lab5: labs\lab5\code\src\Lab5.exe
echo   Lab6: labs\lab6\code\Lab6\src\Lab6.exe
