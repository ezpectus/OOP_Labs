# Lab 4 - Code Refactoring, Multiple Inheritance

## Topic
Graphic editor upgrade: global static MyEditor object (odd variant),
dotted rubber band for all figures, two new shapes built via multiple
inheritance - line with circles (LineShape + EllipseShape) and wireframe
cube (LineShape + RectShape). UML class diagram in the report.

## Variant
Student #13 (odd): **global static** `MyEditor g_editor`. Colors/styles as
in Lab 3 (blue rubber band -> now **dotted**; rect no fill, two corners;
ellipse light-green fill, center->corner). Toolbar: 6 icon buttons with
tooltips, one per figure type.

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
    ├── src/           ← Lab4.cpp, editor.*, toolbar.*, shape.*, point.*, line.*, rect.*, ellipse.*, lineoo.*, cube.*, resource.h
    └── res/           ← Lab4.rc
```

## Build & Run (MinGW / g++)

### Build
```powershell
cd "F:\VSC projects\OOP_Labs\labs\lab4\code\src"
windres -c 65001 --output-format=coff -I . -i ..\res\Lab4.rc -o Lab4.res.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c Lab4.cpp -o Lab4.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c editor.cpp -o editor.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c toolbar.cpp -o toolbar.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c shape.cpp -o shape.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c point.cpp -o point.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c line.cpp -o line.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c rect.cpp -o rect.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c ellipse.cpp -o ellipse.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c lineoo.cpp -o lineoo.o
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c cube.cpp -o cube.o
g++ -municode -mwindows -o Lab4.exe Lab4.o editor.o toolbar.o shape.o point.o line.o rect.o ellipse.o lineoo.o cube.o Lab4.res.o -lgdi32 -luser32 -lcomctl32 -lcomdlg32
```

Or from repo root: `build-lab.bat 4 run`

### Run
```powershell
.\Lab4.exe
```

## Student
Stepanenko Denys, IM-051, 2026
