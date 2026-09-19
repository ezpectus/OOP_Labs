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

## Launch (PowerShell)
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab5\code"
msbuild Lab5.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\Lab5.exe
```

## Student
Stepanenko Denys, IM-051, 2026
