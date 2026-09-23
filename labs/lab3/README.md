# Lab 3 - Toolbar + Message Handlers as Class Members

## Topic
Graphic editor with Toolbar (icon buttons + tooltips), message handlers
as class members, UML class diagram.

## Variant
J = Jlab2 + 1 = 14: static array N=114 (14 mod 3 = 2), solid **blue** rubber
band (14 mod 4 = 2); rectangle - input **by two corners**, **no fill**
(14 mod 5 = 4); ellipse - input **from center to corner**, **light-green fill**
(14 mod 6 = 2); type indicator **in Objects menu** via OnInitMenuPopup
(14 mod 2 = 0).

## Structure
```
lab3/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    ├── Lab3.sln
    ├── Lab3.vcxproj
    ├── src/           ← Lab3.cpp, editor.*, toolbar.*, shape.*, point.*, line.*, rect.*, ellipse.*, resource.h
    └── res/           ← Lab3.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab3\code\src"
windres -c 65001 --output-format=coff -I . -i ..\res\Lab3.rc -o Lab3.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab3.cpp -o Lab3.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c editor.cpp -o editor.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c toolbar.cpp -o toolbar.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab3.exe Lab3.o editor.o toolbar.o shape.o point.o line.o rect.o ellipse.o Lab3.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab3.exe
```

## Student
Stepanenko Denys, IM-051, 2026
