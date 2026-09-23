// Lab3.cpp - main file
// Lab 3: Graphics editor with Shape hierarchy + Toolbar
// Stepanenko Denys, IM-051, 2026
// Variant: J=14 (Jlab2+1), static array N=114, solid blue rubber band,
//   rect: 2 corners + no fill, ellipse: center->corner + light-green fill,
//   type indicator in Objects menu
// Message handlers are members of MyEditor class

#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "editor.h"

static TCHAR szWindowClass[] = _T("Lab3WindowClass");
static TCHAR szTitle[] = _T("Lab 3 - Graphics Editor - Stepanenko Denys, IM-051");

static MyEditor g_editor;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        g_editor.OnCommand(hWnd, wParam);
        break;

    case WM_INITMENUPOPUP:
        g_editor.OnInitMenuPopup(wParam);
        break;

    case WM_LBUTTONDOWN:
        g_editor.OnLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_MOUSEMOVE:
        g_editor.OnMouseMove(hWnd, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_LBUTTONUP:
        g_editor.OnLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
        g_editor.OnPaint(hWnd);
        break;

    case WM_DESTROY:
        g_editor.OnDestroy();
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
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
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
// cd "F:\VSC projects\OOP_Labs\labs\lab3\code\src"
// .\Lab3.exe
