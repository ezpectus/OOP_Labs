# РГР — Варіант 13 (Ж = 13)

**Список студентів групи з розрахунками успішності.**

Win32-програма: таблиця студентів (ListView), додавання/
редагування/видалення через діалоги, сортування, статистика
групи, збереження/завантаження CSV-файлу.

## Класи

- `Student` — ПІБ, № залікової, 5 оцінок; `avg()`, `hasDebt()`,
  `isExcellent()`, `toCSV()/fromCSV()`
- `StudentGroup` — `vector<Student>`; add/update/remove,
  `sortByAvg()/sortByName()`, `groupAvg()`, `successRate()`,
  `excellentCount()`, `debtorCount()`, `saveToFile()/loadFromFile()`

## Структура

```
code/src/student.h/.cpp        — клас студента
code/src/student_group.h/.cpp  — контейнер групи + файл + статистика
code/src/dialogs.h/.cpp        — модальні діалоги (Add/Edit, Stats)
code/src/main.cpp              — WinMain, WndProc, ListView, меню
code/src/resource.h            — ідентифікатори
code/res/RGR.rc                — меню + шаблони діалогів
screenshots/                   — скриншоти програми
report/report.md               — звіт
```

## Збірка (MinGW)

```bat
cd code\src
g++ -c -std=c++17 -DUNICODE -D_UNICODE student.cpp student_group.cpp dialogs.cpp main.cpp
windres -c 65001 --output-format=coff -I . -i ..\res\RGR.rc -o RGR.res.o
g++ -mwindows -municode *.o RGR.res.o -o RGR.exe -lcomctl32
```

## Використання

- **Файл** — новий список / відкрити / зберегти / зберегти як
- **Студент** — додати / редагувати / видалити (Del)
- **Сортування** — за середнім балом або за ПІБ
- **Статистика** — середній бал групи, % успішності,
  відмінники, боржники
