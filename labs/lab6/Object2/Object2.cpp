// Object2.cpp — data generator program
// Lab 6: Creates nPoint (x,y) pairs, shows them, writes to Clipboard
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static TCHAR szClass[] = _T("Object2Class");
static TCHAR szTitle[] = _T("Object 2 — Data Generator");

struct Lab6Params {
    int nPoint;
    int xMin, xMax;
    int yMin, yMax;
    HWND hWndSender;
};

static Lab6Params g_params;
static double* g_x = NULL;
static double* g_y = NULL;
static int g_count = 0;
static HWND g_hWnd = NULL;

static void GenerateData()
{
    if (g_x) { delete[] g_x; g_x = NULL; }
    if (g_y) { delete[] g_y; g_y = NULL; }

    g_count = g_params.nPoint;
    if (g_count <= 0) return;

    g_x = new double[g_count];
    g_y = new double[g_count];

    srand((unsigned)time(NULL));

    for (int i = 0; i < g_count; i++)
    {
        double rx = (double)rand() / RAND_MAX;
        double ry = (double)rand() / RAND_MAX;
        g_x[i] = g_params.xMin + rx * (g_params.xMax - g_params.xMin);
        g_y[i] = g_params.yMin + ry * (g_params.yMax - g_params.yMin);
    }

    InvalidateRect(g_hWnd, NULL, TRUE);
}

static void WriteToClipboard()
{
    if (!g_count || !g_x || !g_y) return;

    // Format: "x\ty\n" per line
    char* buf = new char[g_count * 32];
    int pos = 0;
    for (int i = 0; i < g_count; i++)
    {
        pos += sprintf_s(buf + pos, g_count * 32 - pos, "%.2f\t%.2f\n", g_x[i], g_y[i]);
    }

    HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, pos + 1);
    if (hglb)
    {
        memcpy(GlobalLock(hglb), buf, pos + 1);
        GlobalUnlock(hglb);
        OpenClipboard(g_hWnd);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hglb);
        CloseClipboard();
    }
    delete[] buf;

    // Notify Object3 via SendMessage
    HWND hObj3 = FindWindow(_T("Object3Class"), NULL);
    if (hObj3)
        PostMessage(hObj3, WM_USER + 100, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_hWnd = hWnd;
        g_params.nPoint = 20;
        g_params.xMin = 0; g_params.xMax = 100;
        g_params.yMin = 0; g_params.yMax = 100;
        GenerateData();
        break;

    case WM_COPYDATA:
    {
        COPYDATASTRUCT* cds = (COPYDATASTRUCT*)lParam;
        if (cds && cds->cbData == sizeof(Lab6Params))
        {
            memcpy(&g_params, cds->lpData, sizeof(Lab6Params));
            GenerateData();
            WriteToClipboard();
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        TCHAR header[128];
        _stprintf_s(header, 128, _T("Object 2 — %d points, X:[%d..%d], Y:[%d..%d]"),
            g_count, g_params.xMin, g_params.xMax, g_params.yMin, g_params.yMax);
        TextOut(hdc, 10, 10, header, (int)_tcslen(header));

        for (int i = 0; i < g_count && i < 50; i++)
        {
            TCHAR line[64];
            _stprintf_s(line, 64, _T("[%d] x=%.2f  y=%.2f"), i, g_x[i], g_y[i]);
            TextOut(hdc, 10, 35 + i * 15, line, (int)_tcslen(line));
        }

        if (g_count > 50)
        {
            TCHAR more[32];
            _stprintf_s(more, 32, _T("... (%d more)"), g_count - 50);
            TextOut(hdc, 10, 35 + 50 * 15, more, (int)_tcslen(more));
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        if (g_x) delete[] g_x;
        if (g_y) delete[] g_y;
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
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szClass;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) return 1;

    HWND hWnd = CreateWindow(szClass, szTitle,
        WS_OVERLAPPEDWINDOW, 520, 50, 400, 300,
        NULL, NULL, hInstance, NULL);

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
