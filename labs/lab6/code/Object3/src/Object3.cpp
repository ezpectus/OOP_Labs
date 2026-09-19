// Object3.cpp — graph plotter program
// Lab 6: Reads data from Clipboard, draws graph y=f(x)
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static TCHAR szClass[] = _T("Object3Class");
static TCHAR szTitle[] = _T("Object 3 — Graph Plotter");

static double* g_x = NULL;
static double* g_y = NULL;
static int g_count = 0;
static HWND g_hWnd = NULL;

#define WM_CLIPBOARD_NOTIFY (WM_USER + 100)

static void ReadFromClipboard()
{
    if (g_x) { delete[] g_x; g_x = NULL; }
    if (g_y) { delete[] g_y; g_y = NULL; }
    g_count = 0;

    if (!OpenClipboard(g_hWnd)) return;

    HGLOBAL hglb = GetClipboardData(CF_TEXT);
    if (hglb)
    {
        char* text = (char*)GlobalLock(hglb);
        if (text)
        {
            // Count lines
            int lines = 0;
            char* p = text;
            while (*p) { if (*p == '\n') lines++; p++; }
            if (lines > 0)
            {
                g_count = lines;
                g_x = new double[g_count];
                g_y = new double[g_count];

                p = text;
                for (int i = 0; i < g_count && *p; i++)
                {
                    g_x[i] = atof(p);
                    while (*p && *p != '\t') p++;
                    if (*p == '\t') p++;
                    g_y[i] = atof(p);
                    while (*p && *p != '\n') p++;
                    if (*p == '\n') p++;
                }
            }
            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();

    // Sort by x (ascending)
    for (int i = 0; i < g_count - 1; i++)
        for (int j = i + 1; j < g_count; j++)
            if (g_x[j] < g_x[i])
            {
                double tx = g_x[i]; g_x[i] = g_x[j]; g_x[j] = tx;
                double ty = g_y[i]; g_y[i] = g_y[j]; g_y[j] = ty;
            }

    InvalidateRect(g_hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_hWnd = hWnd;
        break;

    case WM_CLIPBOARD_NOTIFY:
        ReadFromClipboard();
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        // Margins
        int ml = 50, mr = 20, mt = 30, mb = 40;
        int gw = w - ml - mr;
        int gh = h - mt - mb;

        // Title
        TextOut(hdc, 10, 5, _T("Object 3 — Graph y=f(x)"), 23);

        if (g_count < 2)
        {
            TextOut(hdc, 10, 50, _T("Waiting for data from Clipboard..."), 34);
            EndPaint(hWnd, &ps);
            break;
        }

        // Find ranges
        double xMin = g_x[0], xMax = g_x[0];
        double yMin = g_y[0], yMax = g_y[0];
        for (int i = 1; i < g_count; i++)
        {
            if (g_x[i] < xMin) xMin = g_x[i];
            if (g_x[i] > xMax) xMax = g_x[i];
            if (g_y[i] < yMin) yMin = g_y[i];
            if (g_y[i] > yMax) yMax = g_y[i];
        }
        if (xMax == xMin) xMax = xMin + 1;
        if (yMax == yMin) yMax = yMin + 1;

        // Draw axes
        HPEN hAxisPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hAxisPen);

        // X axis (bottom)
        MoveToEx(hdc, ml, mt + gh, NULL);
        LineTo(hdc, ml + gw, mt + gh);
        // Y axis (left)
        MoveToEx(hdc, ml, mt, NULL);
        LineTo(hdc, ml, mt + gh);

        // Axis labels
        TCHAR label[32];
        _stprintf_s(label, 32, _T("%.1f"), xMin);
        TextOut(hdc, ml - 15, mt + gh + 5, label, (int)_tcslen(label));
        _stprintf_s(label, 32, _T("%.1f"), xMax);
        TextOut(hdc, ml + gw - 20, mt + gh + 5, label, (int)_tcslen(label));
        _stprintf_s(label, 32, _T("%.1f"), yMin);
        TextOut(hdc, ml - 35, mt + gh - 7, label, (int)_tcslen(label));
        _stprintf_s(label, 32, _T("%.1f"), yMax);
        TextOut(hdc, ml - 35, mt - 7, label, (int)_tcslen(label));

        TextOut(hdc, ml + gw / 2 - 5, mt + gh + 20, _T("X"), 1);
        TextOut(hdc, ml - 40, mt + gh / 2 - 5, _T("Y"), 1);

        // Draw graph line
        HPEN hGraphPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        SelectObject(hdc, hGraphPen);

        for (int i = 0; i < g_count; i++)
        {
            int px = ml + (int)((g_x[i] - xMin) / (xMax - xMin) * gw);
            int py = mt + gh - (int)((g_y[i] - yMin) / (yMax - yMin) * gh);
            if (i == 0)
                MoveToEx(hdc, px, py, NULL);
            else
                LineTo(hdc, px, py);
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hGraphPen);
        DeleteObject(hAxisPen);

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
        WS_OVERLAPPEDWINDOW, 520, 370, 400, 400,
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
