# Lab 3 — Toolbar, Tooltips, Class Handlers

## Topic
Toolbar with buttons, tooltips, message handlers as class methods, UML class diagrams.

## Variant
J=17 (J_lab2+1): static array N=117, solid red rubber band, rect (center input, gray fill), ellipse (2 corners, no fill), type indicator in window title.

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
    ├── src/           ← Lab3.cpp, shape.*, point.*, line.*, rect.*, ellipse.*, resource.h
    └── res/           ← Lab3.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab3\code\src"
windres --output-format=coff -I . -i ..\res\Lab3.rc -o Lab3.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab3.cpp -o Lab3.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab3.exe Lab3.o shape.o point.o line.o rect.o ellipse.o Lab3.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab3.exe
```

## Student
Stepanenko Denys, IM-051, 2026
