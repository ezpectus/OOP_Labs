// module3.cpp - Work 2, wizard window 2 (variant B2 = 2)
// Buttons: [Yes] -> 2, [< Back] -> -1, [Cancel]/x -> 0
// Stepanenko Denys, IM-051

#include <windows.h>
#include <tchar.h>
#include "module3.h"
#include "module3_resource.h"

static INT_PTR CALLBACK DlgProc3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BTN_YES:
            EndDialog(hDlg, 2);   // Yes - wizard finished
            return TRUE;
        case IDC_BTN_BACK:
            EndDialog(hDlg, -1);  // Back - return to wizard window 1
            return TRUE;
        case IDC_BTN_CANCEL:
            EndDialog(hDlg, 0);   // Cancel - abort wizard
            return TRUE;
        }
    }
    break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        break;

    default:
        return FALSE;
    }
    return FALSE;
}

int Func_MOD3(HWND hWnd)
{
    return (int)DialogBox(
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        MAKEINTRESOURCE(IDD_DIALOG3),
        hWnd, DlgProc3);
}
