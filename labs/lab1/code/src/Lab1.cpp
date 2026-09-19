// Lab1.cpp — main file
// Lab 1: Modular Win32 project
// Stepanenko Denys, IM-051, 2026

#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "module1.h"
#include "module2.h"

#define MAX_TEXT 256

static TCHAR szWindowClass[] = _T("Lab1WindowClass");
static TCHAR szTitle[] = _T("Lab 1 — Stepanenko Denys, IM-051");

static TCHAR g_text[MAX_TEXT] = _T("");
static int   g_number = 0;
static bool  g_showText   = false;
static bool  g_showNumber = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case ID_WORK1:
        {
            TCHAR buf[MAX_TEXT];
            if (Func_MOD1(hWnd, buf, MAX_TEXT) == 1)
            {
                _tcsncpy_s(g_text, MAX_TEXT, buf, _TRUNCATE);
                g_showText = true;
                g_showNumber = false;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;

        case ID_WORK2:
        {
            int val;
            if (Func_MOD2(hWnd, &val) == 1)
            {
                g_number = val;
                g_showNumber = true;
                g_showText = false;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case IDM_ABOUT:
            MessageBox(hWnd,
                _T("Lab 1 — Modular Win32 Project\nStepanenko Denys, IM-051\nVariant: B1=0, B2=1"),
                _T("About"), MB_OK | MB_ICONINFORMATION);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (g_showText)
        {
            TextOut(hdc, 20, 30, _T("Entered text:"), 13);
            TextOut(hdc, 20, 55, g_text, (int)_tcslen(g_text));
        }
        else if (g_showNumber)
        {
            TCHAR buf[32];
            _stprintf_s(buf, 32, _T("Selected number: %d"), g_number);
            TextOut(hdc, 20, 30, buf, (int)_tcslen(buf));
        }
        else
        {
            TextOut(hdc, 20, 30, _T("Select Work -> Work 1 or Work 2 from menu"), 43);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
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
    wcex.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINMENU);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, _T("Failed to register window class"),
                   _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateWindow(
        szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 350,
        NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, _T("Failed to create window"),
                   _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

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

// LAUNCH (PowerShell):
// cd "F:\VSC projects\OOP_Labs\labs\lab1\code"
// msbuild Lab1.sln /p:Configuration=Debug /p:Platform=x64
// .\x64\Debug\Lab1.exe
