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

## Launch (PowerShell)
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab4\code"
msbuild Lab4.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\Lab4.exe
```

## Student
Stepanenko Denys, IM-051, 2026
