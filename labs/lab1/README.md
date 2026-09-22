# Lab 1 — Modular Win32 Project

## Topic
Win32, modular project, two dialog windows from separate modules.

## Variant
J=16: B1=0 (text input dialog), B2=1 (scrollbar dialog).

## Structure
```
lab1/
├── task.md
├── README.md
├── report/           ← звіт (PDF/DOCX)
├── screenshots/      ← скріншоти роботи програми
└── code/
    ├── Lab1.sln
    ├── Lab1.vcxproj
    ├── src/           ← Lab1.cpp, module1.*, module2.*, resource.h
    └── res/           ← Lab1.rc, module1.rc, module2.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab1\code\src"
windres --output-format=coff -I . -i ..\res\Lab1.rc -o Lab1.res.o
windres --output-format=coff -I . -i ..\res\module1.rc -o module1.res.o
windres --output-format=coff -I . -i ..\res\module2.rc -o module2.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab1.cpp -o Lab1.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c module1.cpp -o module1.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c module2.cpp -o module2.o
g++ -municode -o Lab1.exe Lab1.o module1.o module2.o Lab1.res.o module1.res.o module2.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab1.exe
```

## Student
Stepanenko Denys, IM-051, 2026
