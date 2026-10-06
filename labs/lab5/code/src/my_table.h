// my_table.h — independent table module (no project dependencies)
#pragma once
#include <windows.h>

class MyTable
{
private:
    HWND hDlg;
    HWND hList;
    int rowCount;

public:
    MyTable();
    ~MyTable();

    void Activate(HWND hParent, HINSTANCE hInst);
    void Close();
    void Add(const wchar_t* name, int x1, int y1, int x2, int y2);
    void Clear();
    bool IsActive() const { return hDlg != NULL; }
    HWND GetHwnd() const { return hDlg; }
    void SetHwnd(HWND hwnd);
};

// Resource IDs for my_table (kept here for independence)
#define IDD_TABLE_DIALOG  500
#define IDC_TABLE_LIST    501
