// ============================================================
// main.cpp — РГР, варіант 13
// Програма списку студентів групи з розрахунками успішності.
// Головне вікно: ListView (таблиця) + меню Файл/Студент/
// Сортування/Статистика. Діалоги Add/Edit — окремий модуль.
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
        return 0;

    case WM_SIZE:
        MoveWindow(g_hList, 0, 0, LOWORD(lp), HIWORD(lp), TRUE);
        return 0;

    case WM_KEYDOWN:
        if (wp == VK_DELETE) DoRemove(hWnd);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
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
