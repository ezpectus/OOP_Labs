// Object3.cpp — Determinant program (J=13, variant 1)
// On WM_USER+100 reads an n×n matrix from Clipboard (first line = n, then
// n tab-separated rows), computes det(A) by Gaussian elimination, shows it.
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WM_DATA_READY (WM_USER + 100)   // Object2 -> Object3 notification

static TCHAR szWindowClass[] = _T("Object3Class");
static TCHAR szTitle[] = _T("Object3 — Determinant");
static HWND  g_hWnd = NULL;

static int*   g_matrix = NULL;
static int    g_n = 0;
static int    g_detOK = 0;
static double g_det = 0.0;

// Gaussian elimination with partial pivoting — O(n^3).
// det = product of pivots × (-1)^(row swaps). Exact for int matrices.
static double Determinant(int n, const int* m)
{
    double* a = new double[n * n];
    for (int i = 0; i < n * n; i++) a[i] = (double)m[i];

    double det = 1.0;
    int sign = 1;
    for (int col = 0; col < n; col++)
    {
        // partial pivot: max |a[row][col]| below diagonal
        int piv = col;
        for (int r = col + 1; r < n; r++)
            if (fabs(a[r * n + col]) > fabs(a[piv * n + col])) piv = r;
        if (fabs(a[piv * n + col]) < 1e-12) { delete[] a; return 0.0; }
        if (piv != col)
        {
            for (int c = 0; c < n; c++)
            {
                double t = a[col * n + c]; a[col * n + c] = a[piv * n + c]; a[piv * n + c] = t;
            }
            sign = -sign;
        }
        double d = a[col * n + col];
        det *= d;
        for (int r = col + 1; r < n; r++)
        {
            double f = a[r * n + col] / d;
            for (int c = col; c < n; c++)
                a[r * n + c] -= f * a[col * n + c];
        }
    }
    delete[] a;
    return det * sign;
}

static void ReadFromClipboard()
{
    // clipboard may still be held by Object2 right after SetClipboardData —
    // retry briefly instead of giving up on first failure
    int tries = 0;
    while (!OpenClipboard(g_hWnd) && ++tries < 10) Sleep(20);
    if (tries >= 10) return;
    HGLOBAL hglb = GetClipboardData(CF_TEXT);
    if (!hglb) { CloseClipboard(); return; }

    const char* text = (const char*)GlobalLock(hglb);
    if (!text) { CloseClipboard(); return; }

    // copy out — never modify the clipboard's own buffer
    size_t len = strlen(text) + 1;
    char* copy = new char[len];
    memcpy(copy, text, len);
    GlobalUnlock(hglb);
    CloseClipboard();

    // first token = n, then n*n ints
    char* ctx = NULL;
    char* tok = strtok_s(copy, " \t\r\n", &ctx);
    if (!tok) { delete[] copy; return; }
    int n = atoi(tok);
    if (n <= 0 || n > 50) { delete[] copy; return; }

    delete[] g_matrix;
    g_matrix = new int[n * n];
    g_n = n;
    int filled = 0;
    while ((tok = strtok_s(NULL, " \t\r\n", &ctx)) && filled < n * n)
        g_matrix[filled++] = atoi(tok);
    delete[] copy;

    if (filled < n * n) { g_n = 0; delete[] g_matrix; g_matrix = NULL; }
    else
    {
        g_det = Determinant(g_n, g_matrix);
        g_detOK = 1;
    }
    InvalidateRect(g_hWnd, NULL, TRUE);
}

static void DrawResult(HDC hdc, RECT rc)
{
    SetBkMode(hdc, TRANSPARENT);
    TCHAR line[128];

    TextOut(hdc, 10, 8, _T("Determinant of received matrix:"), 31);

    if (g_n == 0)
    {
        TextOut(hdc, 10, 36, _T("waiting for data — run Lab6 manager"), 35);
        return;
    }

    // matrix grid on the left
    int cellW = 40, cellH = 18, top = 40, left = 20;
    int maxShow = g_n < 10 ? g_n : 10;
    for (int r = 0; r < maxShow; r++)
        for (int c = 0; c < maxShow; c++)
        {
            _stprintf_s(line, 128, _T("%d"), g_matrix[r * g_n + c]);
            TextOut(hdc, left + c * cellW, top + r * cellH, line, (int)_tcslen(line));
        }
    if (g_n > maxShow)
        TextOut(hdc, left, top + maxShow * cellH, _T("…"), 1);

    // result
    int ry = top + maxShow * cellH + 16;
    _stprintf_s(line, 128, _T("n = %d"), g_n);
    TextOut(hdc, 10, ry, line, (int)_tcslen(line));

    if (g_detOK)
    {
        // det of an integer matrix is an integer — print without fraction
        _stprintf_s(line, 128, _T("det(A) = %.0f"), g_det);
        HFONT hBig = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, _T("Segoe UI"));
        HFONT hOldF = (HFONT)SelectObject(hdc, hBig);
        SetTextColor(hdc, RGB(160, 0, 0));
        TextOut(hdc, 10, ry + 24, line, (int)_tcslen(line));
        SelectObject(hdc, hOldF);
        DeleteObject(hBig);
        SetTextColor(hdc, RGB(0, 0, 0));
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_hWnd = hWnd;
        break;

    case WM_DATA_READY:
        ReadFromClipboard();
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc; GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
        DrawResult(hdc, rc);
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
        540, 410, 560, 260, NULL, NULL, hInstance, NULL);
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
