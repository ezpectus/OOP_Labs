# Lab 5 — Multi-window UI, Singleton, Table, File Save

## Topic
Multi-window interface, Singleton pattern, independent table module, saving objects to file.

## Variant
J=16 (even): classic Singleton implementation. Independent `my_table` module (no project dependencies). Non-modal table dialog with listbox. File save (tab-separated: name, x1, y1, x2, y2).

## Structure
```
lab5/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    ├── Lab5.sln
    ├── Lab5.vcxproj
    ├── src/           ← Lab5.cpp, shape.*, point.*, line.*, rect.*, ellipse.*, linecircles.h, cube.h, my_editor.*, my_table.*, resource.h
    └── res/           ← Lab5.rc, my_table.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab5\code\src"
windres --output-format=coff -I . -i ..\res\Lab5.rc -o Lab5.res.o
windres --output-format=coff -I . -i ..\res\my_table.rc -o my_table.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab5.cpp -o Lab5.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c my_editor.cpp -o my_editor.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c my_table.cpp -o my_table.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -municode -o Lab5.exe Lab5.o my_editor.o my_table.o shape.o point.o line.o rect.o ellipse.o Lab5.res.o my_table.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab5.exe
```

## Student
Stepanenko Denys, IM-051, 2026
