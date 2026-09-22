# Lab 4 — Refactoring, Multiple Inheritance, MyEditor

## Topic
Refactoring with MyEditor class, multiple inheritance for new shapes (LineWithCircles, CubeWireframe).

## Variant
J=16 (even): dynamic MyEditor object, dashed rubber band. Two new shapes via multiple inheritance:
- **LineWithCircles** — inherits from LineShape + EllipseShape
- **CubeWireframe** — inherits from LineShape + RectShape

## Structure
```
lab4/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    ├── Lab4.sln
    ├── Lab4.vcxproj
    ├── src/           ← Lab4.cpp, shape.*, point.*, line.*, rect.*, ellipse.*, linecircles.h, cube.h, my_editor.*, resource.h
    └── res/           ← Lab4.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab4\code\src"
windres --output-format=coff -I . -i ..\res\Lab4.rc -o Lab4.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab4.cpp -o Lab4.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c my_editor.cpp -o my_editor.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab4.exe Lab4.o my_editor.o shape.o point.o line.o rect.o ellipse.o Lab4.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab4.exe
```

## Student
Stepanenko Denys, IM-051, 2026
