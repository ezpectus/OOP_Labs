// Lab6.cpp — Manager program (J=13, variant 1)
// Launches Object2/Object3, positions windows, sends params via WM_COPYDATA.
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

// Params block shared over WM_COPYDATA (each program defines its own copy —
// programs are fully independent, no shared headers).
struct Lab6Params
{
    int n;              // matrix dimension n×n
    int minVal, maxVal; // element range
    HWND hWndSender;    // manager hwnd — children may answer back
};

#define IDC_ED_N     201
#define IDC_ED_MIN   202
#define IDC_ED_MAX   203
#define IDC_BTN_RUN  210

static HWND hEdN, hEdMin, hEdMax;
static HWND hObj2 = NULL, hObj3 = NULL;

static TCHAR szWindowClass[] = _T("Lab6ManagerClass");
static TCHAR szTitle[] = _T("Lab 6 Manager — Stepanenko Denys, IM-051");

static HWND MakeEdit(HWND hWnd, int x, int y, int w, int id, const TCHAR* def)
{
    HWND h = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), def,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        x, y, w, 22, hWnd, (HMENU)(INT_PTR)id, NULL, NULL);
    return h;
}

static void MakeLabel(HWND hWnd, int x, int y, const TCHAR* text)
{
    CreateWindow(_T("STATIC"), text, WS_CHILD | WS_VISIBLE | SS_LEFT,
        x, y + 3, 120, 18, hWnd, NULL, NULL, NULL);
}

static void LaunchPrograms(HWND hWnd)
{
    // Object2.exe / Object3.exe — same dir as Lab6.exe, or sibling src folders
    TCHAR self[MAX_PATH], dir[MAX_PATH];
    GetModuleFileName(NULL, self, MAX_PATH);
    lstrcpy(dir, self);
    TCHAR* p = _tcsrchr(dir, _T('\\'));
    if (p) *p = 0;

    TCHAR cmd2[MAX_PATH * 2], cmd3[MAX_PATH * 2];

    hObj2 = FindWindow(_T("Object2Class"), NULL);
    hObj3 = FindWindow(_T("Object3Class"), NULL);

    // WinExec is ANSI-only — convert command lines explicitly
    char cmdA[MAX_PATH * 2];
    if (!hObj2)
    {
        _stprintf_s(cmd2, MAX_PATH * 2, _T("\"%ls\\Object2.exe\""), dir);
        WideCharToMultiByte(CP_ACP, 0, cmd2, -1, cmdA, sizeof(cmdA), NULL, NULL);
        if (WinExec(cmdA, SW_SHOWNORMAL) <= 31)   // fallback: sibling build dir
        {
            _stprintf_s(cmd2, MAX_PATH * 2, _T("\"%ls\\..\\Object2\\Object2.exe\""), dir);
            WideCharToMultiByte(CP_ACP, 0, cmd2, -1, cmdA, sizeof(cmdA), NULL, NULL);
            WinExec(cmdA, SW_SHOWNORMAL);
        }
    }
    if (!hObj3)
    {
        _stprintf_s(cmd3, MAX_PATH * 2, _T("\"%ls\\Object3.exe\""), dir);
        WideCharToMultiByte(CP_ACP, 0, cmd3, -1, cmdA, sizeof(cmdA), NULL, NULL);
        if (WinExec(cmdA, SW_SHOWNORMAL) <= 31)
        {
            _stprintf_s(cmd3, MAX_PATH * 2, _T("\"%ls\\..\\Object3\\Object3.exe\""), dir);
            WideCharToMultiByte(CP_ACP, 0, cmd3, -1, cmdA, sizeof(cmdA), NULL, NULL);
            WinExec(cmdA, SW_SHOWNORMAL);
        }
    }

    // wait for their windows to register
    for (int i = 0; i < 50 && (!hObj2 || !hObj3); i++)
    {
        if (!hObj2) hObj2 = FindWindow(_T("Object2Class"), NULL);
        if (!hObj3) hObj3 = FindWindow(_T("Object3Class"), NULL);
        if (hObj2 && hObj3) break;
        Sleep(100);
    }

    // arrange: manager left, matrix top-right, determinant bottom-right
    MoveWindow(hWnd,  50,  50, 460, 280, TRUE);
    if (hObj2) MoveWindow(hObj2, 540,  50, 560, 340, TRUE);
    if (hObj3) MoveWindow(hObj3, 540, 410, 560, 260, TRUE);
}

static void SendParams(HWND hWnd)
{
    TCHAR buf[64];
    Lab6Params params;

    GetWindowText(hEdN,   buf, 64); params.n      = _ttoi(buf);
    GetWindowText(hEdMin, buf, 64); params.minVal = _ttoi(buf);
    GetWindowText(hEdMax, buf, 64); params.maxVal = _ttoi(buf);
    params.hWndSender = hWnd;

    if (params.n <= 0)  params.n = 5;
    if (params.n > 20)  params.n = 20;          // display keeps it sane
    if (params.maxVal <= params.minVal) params.maxVal = params.minVal + 1;

    if (!hObj2) hObj2 = FindWindow(_T("Object2Class"), NULL);
    if (!hObj3) hObj3 = FindWindow(_T("Object3Class"), NULL);

    if (!hObj2)
    {
        MessageBox(hWnd, _T("Object2 window not found — run the system first"),
            _T("Lab6"), MB_OK | MB_ICONWARNING);
        return;
    }

    COPYDATASTRUCT cds;
    cds.dwData = 0;
    cds.cbData = sizeof(Lab6Params);
    cds.lpData = &params;
    SendMessage(hObj2, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        MakeLabel(hWnd, 30, 30,  _T("n (matrix size):"));
        MakeLabel(hWnd, 30, 65,  _T("Min:"));
        MakeLabel(hWnd, 30, 100, _T("Max:"));
        hEdN   = MakeEdit(hWnd, 160, 30,  120, IDC_ED_N,   _T("5"));
        hEdMin = MakeEdit(hWnd, 160, 65,  120, IDC_ED_MIN, _T("0"));
        hEdMax = MakeEdit(hWnd, 160, 100, 120, IDC_ED_MAX, _T("9"));
        CreateWindow(_T("BUTTON"), _T("Run system"),
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            30, 145, 250, 30, hWnd, (HMENU)IDC_BTN_RUN, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_RUN)
        {
            LaunchPrograms(hWnd);
            SendParams(hWnd);
        }
        break;

    case WM_DESTROY:
        // polite shutdown: children close together with the manager
        if (hObj2) PostMessage(hObj2, WM_CLOSE, 0, 0);
        if (hObj3) PostMessage(hObj3, WM_CLOSE, 0, 0);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(szWindowClass, szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        50, 50, 460, 280, NULL, NULL, hInstance, NULL);
    if (!hWnd) return 1;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
