// module2.cpp - Work 2, wizard window 1 (variant B2 = 2)
// Buttons: [Next >] -> 1, [Cancel]/x -> 0
// Stepanenko Denys, IM-051

#include <windows.h>
#include <tchar.h>
#include "module2.h"
#include "module2_resource.h"

static INT_PTR CALLBACK DlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BTN_NEXT:
            EndDialog(hDlg, 1);   // Next - open wizard window 2
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

int Func_MOD2(HWND hWnd)
{
    return (int)DialogBox(
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        MAKEINTRESOURCE(IDD_DIALOG2),
        hWnd, DlgProc2);
}
