// my_table.cpp — independent table module implementation
// No includes from project (only windows.h via my_table.h)
#include "my_table.h"

static MyTable* g_pTable = nullptr;

MyTable::MyTable() : hDlg(NULL), hList(NULL), rowCount(0) {}

MyTable::~MyTable() { Close(); }

void MyTable::SetHwnd(HWND hwnd)
{
    hDlg = hwnd;
    hList = GetDlgItem(hwnd, IDC_TABLE_LIST);
}

void MyTable::Activate(HWND hParent, HINSTANCE hInst)
{
    if (hDlg) { Close(); return; }
    g_pTable = this;
    hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TABLE_DIALOG),
                        hParent, TableDlgProc);
}

void MyTable::Close()
{
    if (hDlg)
    {
        DestroyWindow(hDlg);
        hDlg = NULL;
        hList = NULL;
    }
}

void MyTable::Add(const wchar_t* name, int x1, int y1, int x2, int y2)
{
    if (!hList) return;
    wchar_t buf[256];
    swprintf_s(buf, 256, L"%s\t%d\t%d\t%d\t%d", name, x1, y1, x2, y2);
    SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)buf);
    rowCount++;
    // Auto-scroll to bottom
    SendMessageW(hList, LB_SETTOPINDEX, rowCount - 1, 0);
}

void MyTable::Clear()
{
    if (hList)
    {
        SendMessageW(hList, LB_RESETCONTENT, 0, 0);
        rowCount = 0;
    }
}

// Dialog procedure — static, internal to this module
static INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        if (g_pTable) g_pTable->SetHwnd(hDlg);
        return TRUE;

    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE) { DestroyWindow(hDlg); return TRUE; }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL) { DestroyWindow(hDlg); return TRUE; }
        break;

    case WM_DESTROY:
        if (g_pTable) g_pTable->SetHwnd(NULL);
        break;

    default:
        return FALSE;
    }
    return FALSE;
}
