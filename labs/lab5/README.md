# Lab 5 — Multi-window UI, Singleton, Table, File Save

## Topic
Multi-window interface, Singleton pattern, independent table module, saving objects to file.

## Variant
J=13 (odd): Meyers Singleton (function-local static in getInstance). Independent `my_table` module (no project dependencies). Non-modal table dialog with listbox. File save (tab-separated: name, x1, y1, x2, y2).

## Extra features (beyond spec)
- **Load from file** — File → Load restores all shapes, clears canvas first (`LoadFromFile` → `ClearAll`), rebuilds table via `SyncTableToEditor`
- **Save/Open via common dialogs** — `GetSaveFileNameW`/`GetOpenFileNameW` (`OPENFILENAME`) — user picks path and name, no hardcoded file
- **Custom toolbar icons** — own `res/toolbar.bmp` 9×16px strip loaded via `ImageList` + `ILC_MASK` (magenta = transparent)
- **Row → highlight** — click a table row: the shape gets a red dashed frame (`LBN_SELCHANGE` → `TableRowCallback` → `SelectIndex`)
- **Row → delete** — "Delete selected" button in the table dialog removes the row AND the shape (`NotifyRowDelete` → `DeleteIndex`)
- Table↔editor link uses plain function-pointer callbacks — `my_table` stays fully independent (Observer-style, zero new deps)
- Select tool, Eraser, Fill/Pen color via `ChooseColor`, Del key, Clear canvas, Triangle shape
- Robustness: visible 7px point dots (was SetPixel 1px), zero-size shapes skipped on click-without-drag, `SetCapture` so mouse-up isn't lost outside the window

## Test files
`test_shapes/` — sample shape files for File→Load: `shapes3`, `shapes4` (hand-made sets) plus saved dumps (`shapes2`, `canvs1`, `shapes_default`).

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
    ├── src/           ← Lab5.cpp, shape.*, point.*, line.*, rect.*, ellipse.*, triangle.*, linecircles.h, cube.h, my_editor.*, my_table.*, resource.h
    └── res/           ← Lab5.rc, my_table.rc, toolbar.bmp
test_shapes/           ← sample files for File→Load
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
g++ -std=c++26 -DUNICODE -D_UNICODE -municode -O2 -c triangle.cpp -o triangle.o
g++ -municode -o Lab5.exe Lab5.o my_editor.o my_table.o shape.o point.o line.o rect.o ellipse.o triangle.o Lab5.res.o my_table.res.o -lgdi32 -luser32 -lcomctl32
```

### Run
```powershell
.\Lab5.exe
```

## Student
Stepanenko Denys, IM-051, 2026
