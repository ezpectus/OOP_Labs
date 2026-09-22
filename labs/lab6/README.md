# Lab 6 — Three Independent Programs, IPC

## Topic
Three independent programs communicating via WM_COPYDATA and Clipboard.

## Variant
J=16 mod 4 = 0:
- **Lab6 (Manager)** — user inputs nPoint, xMin, xMax, yMin, yMax; launches Object2 and Object3; positions windows; sends params via WM_COPYDATA.
- **Object2** — generates random (x,y) pairs in given ranges; displays values; writes to Clipboard; notifies Object3.
- **Object3** — reads from Clipboard; draws graph y=f(x) with axes and labels.

## Structure
```
lab6/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    ├── Lab6.sln       ← Solution with 3 projects
    ├── Lab6/          ← Manager program
    │   ├── Lab6.vcxproj
    │   ├── src/       ← Lab6.cpp, resource.h
    │   └── res/       ← Lab6.rc
    ├── Object2/       ← Data generator
    │   ├── Object2.vcxproj
    │   └── src/       ← Object2.cpp
    └── Object3/       ← Graph plotter
        ├── Object3.vcxproj
        └── src/       ← Object3.cpp
```

## Build & Run (MinGW / g++)

### Build all 3 programs
```powershell
# Lab6 Manager
cd "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src"
windres --output-format=coff -I . -i ..\res\Lab6.rc -o Lab6.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab6.cpp -o Lab6.o
g++ -municode -o Lab6.exe Lab6.o Lab6.res.o -lgdi32 -luser32 -lcomctl32

# Object2
cd "F:\VSC projects\OOP_Labs\labs\lab6\code\Object2\src"
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Object2.cpp -o Object2.o
g++ -municode -o Object2.exe Object2.o -lgdi32 -luser32

# Object3
cd "F:\VSC projects\OOP_Labs\labs\lab6\code\Object3\src"
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Object3.cpp -o Object3.o
g++ -municode -o Object3.exe Object3.o -lgdi32 -luser32

# Copy Object2.exe and Object3.exe next to Lab6.exe
copy "F:\VSC projects\OOP_Labs\labs\lab6\code\Object2\src\Object2.exe" "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src\"
copy "F:\VSC projects\OOP_Labs\labs\lab6\code\Object3\src\Object3.exe" "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src\"
```

### Run
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src"
.\Lab6.exe
```

Object2 and Object3 are launched automatically by Lab6 via CreateProcess.

## Student
Stepanenko Denys, IM-051, 2026
