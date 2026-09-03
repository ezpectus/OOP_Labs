# OOP_Labs

Лабораторні роботи з ООП, КПІ ім. Ігоря Сікорського

Степаненко Денис, ІМ-051, 121 «Інженерія програмного забезпечення»

Викладач: доц. Порєв В.М.

## Лабораторні

Всі лаби на C++ у Visual Studio (Win32). Кожна наступна лаба продовжує попередню — в результаті виходить графічний редактор об'єктів.

| # | Лаба | Тема | Статус |
|---|------|------|--------|
| 1 | lab1 | Win32, модульний проєкт, два вікна діалогу з окремих модулів | ⬜ Не почата |
| 2 | lab2 | Ієрархія класів Shape, поліморфізм, малювання фігур мишею | ⬜ Не почата |
| 3 | lab3 | Toolbar, tooltips, обробники як методи класу, діаграми UML | ⬜ Не почата |
| 4 | lab4 | Рефакторинг, множинне успадкування, клас MyEditor | ⬜ Не почата |
| 5 | lab5 | Багатовіконний інтерфейс, Singleton, таблиця об'єктів, запис у файл | ⬜ Не почата |
| 6 | lab6 | Три незалежні програми, обмін через WM_COPYDATA та Clipboard | ⬜ Не почата |

**Легенда:** ⬜ Не почата | 🔄 В процесі | ✅ Готова

В кожній папці `labN/` є `task.md` — там умова, теорія і відповіді на контрольні запитання.

## Структура коду

```
labs/
├── lab1/
│   ├── Lab1.sln
│   ├── Lab1.vcxproj
│   ├── Lab1.cpp          ← головний файл (коментарі запуску в кінці)
│   ├── module1.cpp / .h / .rc
│   ├── module2.cpp / .h / .rc
│   └── resource.h
├── lab2/
│   ├── Lab2.sln
│   ├── Lab2.cpp
│   ├── shape.h / .cpp
│   ├── point.h / .cpp
│   └── ...
├── ...lab3, lab4, lab5...
└── lab6/
    ├── Lab6.sln           ← Solution з 3 проєктів
    ├── Lab6/Lab6.cpp
    ├── Object2/Object2.cpp
    └── Object3/Object3.cpp
```

## Запуск

У кінці кожного головного `.cpp` файлу — закоментовані команди для термінала:

```cpp
// ЗАПУСК (PowerShell):
// cd "F:\VSC projects\OOP_Labs\labs\lab1"
// msbuild Lab1.sln /p:Configuration=Debug /p:Platform=x64
// .\x64\Debug\Lab1.exe
```

Для Lab6 — три програми (Object2 та Object3 запускаються автоматично через WinExec):

```cpp
// ЗАПУСК Lab6 (PowerShell):
// cd "F:\VSC projects\OOP_Labs\labs\lab6"
// msbuild Lab6.sln /p:Configuration=Debug /p:Platform=x64
// .\x64\Debug\Lab6.exe
```

Код пишемо в VS Code, компілюємо через термінал (msbuild) або відкривши `.sln` у Visual Studio.

## Технології

C++ (C++26, /std:c++latest), Visual Studio 2022, Win32 API

## Структура

```
labs/lab1..lab6/   — лабораторні (task.md + код)
oop_docs/          — силабус, методичка, варіанти РГР
```
