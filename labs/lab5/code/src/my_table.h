// my_table.h — independent table module (no project dependencies)
#pragma once
#include <windows.h>

// Observer-style hook: table reports a row index, the owner decides
// what it means. my_table never knows about shapes or the editor.
typedef void (*TableRowCallback)(int rowIndex);

class MyTable
{
private:
    HWND hDlg;
    HWND hList;
    int rowCount;
    TableRowCallback onRowSelect;
    TableRowCallback onRowDelete;

public:
    MyTable();
    ~MyTable();

    void Activate(HWND hParent, HINSTANCE hInst);
    void Close();
    void Add(const wchar_t* name, int x1, int y1, int x2, int y2);
    void Remove(int index);   // erase row (eraser tool sync)
    void Clear();
    bool IsActive() const { return hDlg != NULL; }
    HWND GetHwnd() const { return hDlg; }
    void SetHwnd(HWND hwnd);

    // event hooks — plain function pointers, zero dependencies
    void SetOnRowSelect(TableRowCallback cb) { onRowSelect = cb; }
    void SetOnRowDelete(TableRowCallback cb) { onRowDelete = cb; }
    void NotifyRowSelect(int index) { if (onRowSelect) onRowSelect(index); }
    void NotifyRowDelete(int index) { if (onRowDelete) onRowDelete(index); }
    int  GetSelectedRow() const;   // current listbox selection or -1
};

// Resource IDs for my_table (kept here for independence)
#define IDD_TABLE_DIALOG  500
#define IDC_TABLE_LIST    501
#define IDC_TABLE_DELETE  502
