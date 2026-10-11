# Lab 6 — Multi-program system, IPC via Windows API

## Topic
Three independent Win32 programs exchanging data through WM_COPYDATA + Clipboard: manager, matrix generator, determinant calculator.

## Variant
J=13 → 13 mod 4 = 1: `Lab6` (manager, fields n/Min/Max) launches `Object2` (random n×n int matrix → Clipboard CF_TEXT) and `Object3` (reads matrix, computes det(A) via Gaussian elimination, shows result). Programs are fully independent — no shared headers, communication only through `WM_COPYDATA`, `CF_TEXT` Clipboard and `PostMessage(WM_USER+100)`.

## Structure
```
lab6/
├── task.md
├── README.md
├── report/
├── screenshots/
└── code/
    └── src/
        ├── Lab6/      ← Lab6.cpp (manager: inputs, WinExec, WM_COPYDATA)
        ├── Object2/   ← Object2.cpp (matrix generator → Clipboard)
        └── Object3/   ← Object3.cpp (Clipboard → Gaussian det(A))
```

## Build & Run (MinGW / g++)

### Build
```powershell
build-lab.bat 6 Lab6
build-lab.bat 6 Object2
build-lab.bat 6 Object3
```

### Run
```powershell
.\code\src\Lab6\Lab6.exe    # enter n, Min, Max → Run system
```

## Student
Stepanenko Denys, IM-051, 2026
