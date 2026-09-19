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

## Launch (PowerShell)
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab1\code"
msbuild Lab1.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\Lab1.exe
```

## Student
Stepanenko Denys, IM-051, 2026
