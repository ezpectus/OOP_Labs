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

static int SelectedRow()
{
    return ListView_GetNextItem(g_hList, -1, LVNI_SELECTED);
}

// ---------- дії ----------

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
        return 0;

    case WM_SIZE:
        MoveWindow(g_hList, 0, 0, LOWORD(lp), HIWORD(lp), TRUE);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDM_FILE_EXIT: DestroyWindow(hWnd); return 0;
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
