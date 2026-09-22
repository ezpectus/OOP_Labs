# Lab 2 — Shape Class Hierarchy

## Topic
Shape class hierarchy, polymorphism, mouse drawing with rubber band.

## Variant
J=16: static array N=116, solid black rubber band, rect (2 corners, orange fill), ellipse (center input, white fill), type indicator in menu.

## Structure
```
lab2/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    ├── Lab2.sln
    ├── Lab2.vcxproj
    ├── src/           ← Lab2.cpp, shape.*, point.*, line.*, rect.*, ellipse.*, resource.h
    └── res/           ← Lab2.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab2\code\src"
windres --output-format=coff -I . -i ..\res\Lab2.rc -o Lab2.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab2.cpp -o Lab2.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab2.exe Lab2.o shape.o point.o line.o rect.o ellipse.o Lab2.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab2.exe
```

## Student
Stepanenko Denys, IM-051, 2026
