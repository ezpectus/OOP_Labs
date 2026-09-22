// Lab6.cpp — main manager program
// Lab 6: Three independent programs, IPC via WM_COPYDATA + Clipboard
// Stepanenko Denys, IM-051, 2026
// Variant: 0 (J=16 mod 4=0) — nPoint, xMin, xMax, yMin, yMax

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

static TCHAR szClass[] = _T("Lab6Manager");
static TCHAR szTitle[] = _T("Lab 6 — Manager — Stepanenko Denys, IM-051");

static HWND hObj2 = NULL;
static HWND hObj3 = NULL;

#define IDC_EDIT_NPOINT  201
#define IDC_EDIT_XMIN    202
#define IDC_EDIT_XMAX    203
#define IDC_EDIT_YMIN    204
#define IDC_EDIT_YMAX    205
#define IDC_BTN_RUN      206
#define IDC_BTN_CLOSE    207
#define IDD_MAIN_DLG     200

struct Lab6Params {
    int nPoint;
    int xMin, xMax;
    int yMin, yMax;
    HWND hWndSender;
};

static void LaunchPrograms(HWND hWnd)
{
    TCHAR path2[MAX_PATH], path3[MAX_PATH];
    GetModuleFileName(NULL, path2, MAX_PATH);

    // Extract directory
    TCHAR* p = _tcsrchr(path2, _T('\\'));
    if (p) *p = 0;

    swprintf(path3, MAX_PATH, L"%s\\Object2.exe", path2);
    TCHAR path4[MAX_PATH];
    swprintf(path4, MAX_PATH, L"%s\\Object3.exe", path2);

    // Check if already running
    hObj2 = FindWindow(_T("Object2Class"), NULL);
    hObj3 = FindWindow(_T("Object3Class"), NULL);

    if (!hObj2)
    {
        TCHAR cmd[512];
        swprintf(cmd, 512, L"\"%s\" %lld", path3, (long long)hWnd);
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        if (pi.hProcess) CloseHandle(pi.hProcess);
        if (pi.hThread) CloseHandle(pi.hThread);
    }

    if (!hObj3)
    {
        TCHAR cmd[512];
        swprintf(cmd, 512, L"\"%s\" %lld", path4, (long long)hWnd);
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        if (pi.hProcess) CloseHandle(pi.hProcess);
        if (pi.hThread) CloseHandle(pi.hThread);
    }

    // Wait a moment for windows to appear
    for (int i = 0; i < 50; i++)
    {
        if (!hObj2) hObj2 = FindWindow(_T("Object2Class"), NULL);
        if (!hObj3) hObj3 = FindWindow(_T("Object3Class"), NULL);
        if (hObj2 && hObj3) break;
        Sleep(100);
    }

    // Position windows
    if (hObj2) { MoveWindow(hObj2, 520, 50, 400, 300, TRUE); ShowWindow(hObj2, SW_SHOWNORMAL); }
    if (hObj3) { MoveWindow(hObj3, 520, 370, 400, 400, TRUE); ShowWindow(hObj3, SW_SHOWNORMAL); }
    MoveWindow(hWnd, 50, 50, 450, 350, TRUE);
}

static void SendParams(HWND hWnd)
{
    TCHAR buf[32];
    Lab6Params params;
    params.hWndSender = hWnd;

    GetDlgItemText(hWnd, IDC_EDIT_NPOINT, buf, 32);
    params.nPoint = _ttoi(buf);
    if (params.nPoint < 1) params.nPoint = 10;
    if (params.nPoint > 1000) params.nPoint = 1000;

    GetDlgItemText(hWnd, IDC_EDIT_XMIN, buf, 32);
    params.xMin = _ttoi(buf);

    GetDlgItemText(hWnd, IDC_EDIT_XMAX, buf, 32);
    params.xMax = _ttoi(buf);

    GetDlgItemText(hWnd, IDC_EDIT_YMIN, buf, 32);
    params.yMin = _ttoi(buf);

    GetDlgItemText(hWnd, IDC_EDIT_YMAX, buf, 32);
    params.yMax = _ttoi(buf);

    // Send params to Object2 via WM_COPYDATA
    if (hObj2)
    {
        COPYDATASTRUCT cds;
        cds.dwData = 0;
        cds.cbData = sizeof(Lab6Params);
        cds.lpData = &params;
        SendMessage(hObj2, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
    }
}

static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowText(hDlg, szTitle);
        SetDlgItemText(hDlg, IDC_EDIT_NPOINT, _T("20"));
        SetDlgItemText(hDlg, IDC_EDIT_XMIN, _T("0"));
        SetDlgItemText(hDlg, IDC_EDIT_XMAX, _T("100"));
        SetDlgItemText(hDlg, IDC_EDIT_YMIN, _T("0"));
        SetDlgItemText(hDlg, IDC_EDIT_YMAX, _T("100"));
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_RUN:
            LaunchPrograms(hDlg);
            SendParams(hDlg);
            break;
        case IDC_BTN_CLOSE:
            // Close child programs
            if (hObj2) PostMessage(hObj2, WM_CLOSE, 0, 0);
            if (hObj3) PostMessage(hObj3, WM_CLOSE, 0, 0);
            EndDialog(hDlg, 0);
            break;
        case IDCANCEL:
            if (hObj2) PostMessage(hObj2, WM_CLOSE, 0, 0);
            if (hObj3) PostMessage(hObj3, WM_CLOSE, 0, 0);
            EndDialog(hDlg, 0);
            break;
        }
        break;

    case WM_CLOSE:
        if (hObj2) PostMessage(hObj2, WM_CLOSE, 0, 0);
        if (hObj3) PostMessage(hObj3, WM_CLOSE, 0, 0);
        EndDialog(hDlg, 0);
        break;

    default:
        return FALSE;
    }
    return FALSE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Use a dialog as main window
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DLG), NULL, DlgProc);
    return 0;
}

// LAUNCH (PowerShell):
// cd "F:\VSC projects\OOP_Labs\labs\lab6\code\Lab6\src"
// .\Lab6.exe
