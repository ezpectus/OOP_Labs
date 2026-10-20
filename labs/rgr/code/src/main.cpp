// ============================================================
// main.cpp — РГР, варіант 13
// Програма списку студентів групи з розрахунками успішності.
// Головне вікно: ListView (таблиця) + меню Файл/Студент/
// Сортування/Статистика. Діалоги Add/Edit — окремий модуль.
//
// Build & run (MinGW, з папки code\src):
//   g++ -c -std=c++14 -DUNICODE -D_UNICODE *.cpp
//   windres -c 65001 --output-format=coff -I . -i ..\res\RGR.rc -o RGR.res.o
//   g++ -mwindows -municode *.o -o RGR.exe -lcomctl32
//   .\RGR.exe
// (точні команди — у labs\rgr\README.md)
//
// Архітектура (модель ↔ UI):
//   student.h/.cpp        — модель: дані студента + розрахунки,
//                           жодного Win32 — чисте ООП
//   student_group.h/.cpp  — контейнер vector<Student>: сортування,
//                           статистика, save/load CSV
//   dialogs.h/.cpp        — модальні діалоги (DialogBoxParam+EditCtx)
//   main.cpp              — UI-шар: вікно, ListView, меню, WndProc.
//                           Працює тільки через публічні методи групи.
// ============================================================
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "student.h"
#include "student_group.h"
#include "dialogs.h"

static StudentGroup g_group;
static HWND g_hList = nullptr;
static wchar_t g_filePath[MAX_PATH] = L"";

// ---------- ListView ----------

static void InitListView(HWND hList)
{
    ListView_SetExtendedListViewStyle(hList,
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    const wchar_t* titles[] = {
        L"№", L"ПІБ", L"Залікова", L"Оц.1", L"Оц.2", L"Оц.3",
        L"Оц.4", L"Оц.5", L"Сер.бал", L"Статус"
    };
    const int widths[] = { 35, 160, 75, 45, 45, 45, 45, 45, 60, 90 };

    for (int i = 0; i < 10; i++)
    {
        LVCOLUMNW col = {};
        col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        col.cx = widths[i];
        col.pszText = const_cast<LPWSTR>(titles[i]);
        col.iSubItem = i;
        ListView_InsertColumn(hList, i, &col);
    }
}

static void RefreshList()
{
    ListView_DeleteAllItems(g_hList);
    wchar_t buf[32];
    for (size_t i = 0; i < g_group.size(); i++)
    {
        const Student* s = g_group.at(i);

        LVITEMW it = {};
        it.mask = LVIF_TEXT;
        it.iItem = (int)i;
        swprintf_s(buf, L"%d", (int)i + 1);
        it.pszText = buf;
        ListView_InsertItem(g_hList, &it);

        ListView_SetItemText(g_hList, (int)i, 1,
            const_cast<LPWSTR>(s->getName().c_str()));
        ListView_SetItemText(g_hList, (int)i, 2,
            const_cast<LPWSTR>(s->getBookNo().c_str()));
        for (int g = 0; g < Student::GRADE_COUNT; g++)
        {
            swprintf_s(buf, L"%d", s->getGrade(g));
            ListView_SetItemText(g_hList, (int)i, 3 + g, buf);
        }
        swprintf_s(buf, L"%.1f", s->avg());
        ListView_SetItemText(g_hList, (int)i, 8, buf);

        const wchar_t* status = s->isExcellent() ? L"Відмінник" :
                                (s->hasDebt()   ? L"Борги" : L"ОК");
        ListView_SetItemText(g_hList, (int)i, 9, const_cast<LPWSTR>(status));
    }
}

static int SelectedRow()
{
    return ListView_GetNextItem(g_hList, -1, LVNI_SELECTED);
}

// ---------- дії ----------

static void UpdateTitle(HWND hWnd)
{
    wchar_t title[MAX_PATH + 64];
    if (g_filePath[0])
        swprintf_s(title, L"Група студентів — %s", g_filePath);
    else
        swprintf_s(title, L"Група студентів — новий список");
    SetWindowTextW(hWnd, title);
}

static bool AskSavePath(HWND hWnd)
{
    wchar_t path[MAX_PATH] = L"group.csv";
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"CSV файли (*.csv)\0*.csv\0Усі файли (*.*)\0*.*\0";
    ofn.lpstrDefExt = L"csv";
    ofn.Flags = OFN_OVERWRITEPROMPT;
    if (!GetSaveFileNameW(&ofn)) return false;
    wcscpy_s(g_filePath, path);
    return true;
}

static void DoSave(HWND hWnd, bool saveAs)
{
    if (saveAs || !g_filePath[0])
        if (!AskSavePath(hWnd)) return;
    if (g_group.saveToFile(g_filePath))
        UpdateTitle(hWnd);
    else
        MessageBoxW(hWnd, L"Не вдалося зберегти файл!", L"Помилка", MB_ICONERROR);
}

static void DoOpen(HWND hWnd)
{
    wchar_t path[MAX_PATH] = L"";
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"CSV файли (*.csv)\0*.csv\0Усі файли (*.*)\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameW(&ofn)) return;

    if (g_group.loadFromFile(path))
    {
        wcscpy_s(g_filePath, path);
        RefreshList();
        UpdateTitle(hWnd);
    }
    else
        MessageBoxW(hWnd, L"Не вдалося відкрити файл!", L"Помилка", MB_ICONERROR);
}

static void DoAdd(HWND hWnd)
{
    Student s;
    if (StudentEditDialog(hWnd, L"Додати студента", s))
    {
        g_group.add(s);
        RefreshList();
    }
}

static void DoEdit(HWND hWnd)
{
    int row = SelectedRow();
    if (row < 0) return;
    Student s = *g_group.at((size_t)row);
    if (StudentEditDialog(hWnd, L"Редагувати студента", s))
    {
        g_group.updateAt((size_t)row, s);
        RefreshList();
    }
}

static void DoRemove(HWND hWnd)
{
    int row = SelectedRow();
    if (row < 0) return;
    if (MessageBoxW(hWnd, L"Видалити вибраного студента?",
                    L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        g_group.removeAt((size_t)row);
        RefreshList();
    }
}

static void SeedDemo()
{
    const wchar_t* names[] = {
        L"Шевченко Іван", L"Коваленко Марія", L"Бондар Петро",
        L"Мельник Олена", L"Ткач Андрій"
    };
    const wchar_t* books[] = {
        L"ІМ-051-01", L"ІМ-051-02", L"ІМ-051-03", L"ІМ-051-04", L"ІМ-051-05"
    };
    const int gr[][5] = {
        {95, 92, 91, 90, 94}, {78, 82, 75, 80, 85}, {55, 60, 48, 72, 51},
        {90, 91, 93, 92, 90}, {64, 70, 58, 66, 74}
    };
    for (int i = 0; i < 5; i++)
    {
        Student s(names[i], books[i]);
        for (int g = 0; g < 5; g++) s.setGrade(g, gr[i][g]);
        g_group.add(s);
    }
}

// ---------- WndProc ----------

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        g_hList = CreateWindowExW(0, WC_LISTVIEWW, L"",
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
            0, 0, 0, 0, hWnd, (HMENU)IDC_LIST,
            GetModuleHandleW(nullptr), nullptr);
        InitListView(g_hList);
        SeedDemo();
        RefreshList();
        UpdateTitle(hWnd);
        return 0;

    case WM_SIZE:
        MoveWindow(g_hList, 0, 0, LOWORD(lp), HIWORD(lp), TRUE);
        return 0;

    case WM_KEYDOWN:
        if (wp == VK_DELETE) DoRemove(hWnd);
        return 0;

    case WM_NOTIFY:
        // focus usually sits on the ListView, so VK_DELETE arrives
        // there, not at WndProc — ListView forwards it as LVN_KEYDOWN
        if (((LPNMHDR)lp)->code == LVN_KEYDOWN
            && ((LPNMHDR)lp)->idFrom == IDC_LIST
            && ((LPNMLVKEYDOWN)lp)->wVKey == VK_DELETE)
            DoRemove(hWnd);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDM_FILE_NEW:
            g_group.clear();
            g_filePath[0] = 0;
            RefreshList();
            UpdateTitle(hWnd);
            return 0;
        case IDM_FILE_OPEN:   DoOpen(hWnd);            return 0;
        case IDM_FILE_SAVE:   DoSave(hWnd, false);     return 0;
        case IDM_FILE_SAVEAS: DoSave(hWnd, true);      return 0;
        case IDM_FILE_EXIT:   DestroyWindow(hWnd);     return 0;
        case IDM_ST_ADD:      DoAdd(hWnd);             return 0;
        case IDM_ST_EDIT:     DoEdit(hWnd);            return 0;
        case IDM_ST_REMOVE:   DoRemove(hWnd);          return 0;
        case IDM_SORT_AVG:
            g_group.sortByAvg(false);
            RefreshList();
            return 0;
        case IDM_SORT_NAME:
            g_group.sortByName();
            RefreshList();
            return 0;
        case IDM_STATS_SHOW:  StatsDialog(hWnd, g_group); return 0;
        case IDM_ABOUT:
            MessageBoxW(hWnd,
                L"РГР, варіант 13 (Ж = 13)\n"
                L"Список студентів групи з розрахунками успішності.\n"
                L"Win32 + ListView, класи Student/StudentGroup, CSV-файл.",
                L"Про програму", MB_OK | MB_ICONINFORMATION);
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wp, lp);
}

// ---------- WinMain ----------

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    const wchar_t* cls = L"RGR_GroupList";
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = cls;
    wc.lpszMenuName = L"RGR_MENU";
    RegisterClassExW(&wc);

    HWND hWnd = CreateWindowExW(0, cls, L"Група студентів",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        720, 420, nullptr, nullptr, hInst, nullptr);
    ShowWindow(hWnd, nCmdShow);

    MSG m;
    while (GetMessageW(&m, nullptr, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessageW(&m);
    }
    return 0;
}
