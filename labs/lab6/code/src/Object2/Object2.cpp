// Object2.cpp — Matrix generator program (J=13, variant 1)
// Receives Lab6Params via WM_COPYDATA, generates n×n int matrix in [Min..Max],
// shows it in the window, writes CF_TEXT to Clipboard, notifies Object3.
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Lab6Params
{
    int n;
    int minVal, maxVal;
    HWND hWndSender;
};

#define WM_DATA_READY (WM_USER + 100)   // Object2 -> Object3 notification

static TCHAR szWindowClass[] = _T("Object2Class");
static TCHAR szTitle[] = _T("Object2 — Matrix Generator");
static HWND  g_hWnd = NULL;

static Lab6Params g_params = { 0, 0, 0, NULL };
static int*  g_matrix = NULL;   // flat n×n
static int   g_n = 0;

static void GenerateMatrix()
{
    delete[] g_matrix; g_matrix = NULL;
    g_n = g_params.n;
    if (g_n <= 0) return;

    g_matrix = new int[g_n * g_n];

    srand((unsigned)time(NULL));
    int span = g_params.maxVal - g_params.minVal + 1;
    for (int i = 0; i < g_n * g_n; i++)
        g_matrix[i] = g_params.minVal + rand() % span;

    InvalidateRect(g_hWnd, NULL, TRUE);
}

static void WriteToClipboard()
{
    if (g_n <= 0 || !g_matrix) return;

    // format: first line = n, then n rows of tab-separated ints
    int cap = g_n * g_n * 12 + 64;
    char* buf = new char[cap];
    int pos = sprintf_s(buf, cap, "%d\n", g_n);
    for (int r = 0; r < g_n; r++)
    {
        for (int c = 0; c < g_n; c++)
            pos += sprintf_s(buf + pos, cap - pos, "%d\t", g_matrix[r * g_n + c]);
        pos += sprintf_s(buf + pos, cap - pos, "\n");
    }

    HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, pos + 1);
    if (!hglb) { delete[] buf; return; }
    char* dst = (char*)GlobalLock(hglb);
    memcpy(dst, buf, pos + 1);
    GlobalUnlock(hglb);
    delete[] buf;

    if (OpenClipboard(g_hWnd))
    {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hglb);   // clipboard owns hglb now
        CloseClipboard();
    }
    else
        GlobalFree(hglb);
}

static void NotifyObject3()
{
    HWND hObj3 = FindWindow(_T("Object3Class"), NULL);
    if (hObj3)
        PostMessage(hObj3, WM_DATA_READY, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_hWnd = hWnd;
        break;

    case WM_COPYDATA:
    {
        COPYDATASTRUCT* cds = (COPYDATASTRUCT*)lParam;
        if (cds && cds->cbData == sizeof(Lab6Params))
        {
            memcpy(&g_params, cds->lpData, sizeof(Lab6Params));
            GenerateMatrix();
            WriteToClipboard();
            NotifyObject3();
        }
        return TRUE;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 10, 8, _T("Generated matrix:"), 17);

        TCHAR line[128];
        if (g_n == 0)
        {
            TextOut(hdc, 10, 36, _T("waiting for params from Lab6 manager..."), 38);
        }
        else
        {
            _stprintf_s(line, 128, _T("%d x %d   range [%d..%d]"),
                g_n, g_n, g_params.minVal, g_params.maxVal);
            TextOut(hdc, 10, 36, line, (int)_tcslen(line));

            // draw the matrix as a grid, cells ~40×20 px
            int cellW = 40, cellH = 20, top = 62, left = 20;
            for (int r = 0; r < g_n; r++)
                for (int c = 0; c < g_n; c++)
                {
                    _stprintf_s(line, 128, _T("%d"), g_matrix[r * g_n + c]);
                    TextOut(hdc, left + c * cellW, top + r * cellH,
                        line, (int)_tcslen(line));
                }
            TextOut(hdc, 10, top + g_n * cellH + 10,
                _T("(copied to Clipboard, Object3 notified)"), 39);
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        delete[] g_matrix;
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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        540, 50, 560, 340, NULL, NULL, hInstance, NULL);
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
