// module1.cpp — Work 1: text input dialog (variant B1 = 0)
// Stepanenko Denys, IM-051

#include <windows.h>
#include <tchar.h>
#include "module1.h"
#include "module1_resource.h"

static INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT_TEXT, _T(""));
        return TRUE;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BTN_OK:
        {
            TCHAR buf[MAX_PATH];
            GetDlgItemText(hDlg, IDC_EDIT_TEXT, buf, MAX_PATH);
            // Store text via EndDialog — pass pointer through result
            static TCHAR s_result[MAX_PATH];
            _tcsncpy_s(s_result, MAX_PATH, buf, _TRUNCATE);
            EndDialog(hDlg, (INT_PTR)s_result);
            return TRUE;
        }
        case IDC_BTN_CANCEL:
            EndDialog(hDlg, 0);
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

int Func_MOD1(HWND hWnd, TCHAR* outText, int maxLen)
{
    INT_PTR result = DialogBox(
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        MAKEINTRESOURCE(IDD_DIALOG1),
        hWnd, DlgProc1);

    if (result == 0)
        return 0;  // Cancel pressed

    TCHAR* text = (TCHAR*)result;
    _tcsncpy_s(outText, maxLen, text, _TRUNCATE);
    return 1;  // OK pressed
}
