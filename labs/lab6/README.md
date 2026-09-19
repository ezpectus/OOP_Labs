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

## Launch (PowerShell)
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab6\code"
msbuild Lab6.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\Lab6.exe
```

Object2 and Object3 are launched automatically by Lab6 via WinExec.

## Student
Stepanenko Denys, IM-051, 2026
