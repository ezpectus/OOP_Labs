// Lab2.cpp — main file
// Lab 2: Graphics editor with Shape hierarchy
// Stepanenko Denys, IM-051, 2026
// Variant: J=16, static array N=116, solid black rubber band,
//   rect: 2 corners + orange fill, ellipse: center + white fill,
//   type indicator in menu

#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "shape.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"

#define N 116

static TCHAR szWindowClass[] = _T("Lab2WindowClass");
static TCHAR szTitle[] = _T("Lab 2 — Graphics Editor — Stepanenko Denys, IM-051");

static Shape* pcshape[N];
static int shapeCount = 0;

static int currentType = IDM_POINT;
static bool isDrawing = false;
static Shape* pTempShape = NULL;

static void SetMenuCheckmarks(HWND hWnd, int selectedId)
{
    HMENU hMenu = GetMenu(hWnd);
    CheckMenuItem(hMenu, IDM_POINT,   MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_LINE,    MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_RECT,    MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_ELLIPSE, MF_UNCHECKED);
    CheckMenuItem(hMenu, selectedId,  MF_CHECKED);
}

static Shape* CreateShape(int type, int x1, int y1, int x2, int y2)
{
    switch (type)
    {
    case IDM_POINT:   return new PointShape(x1, y1, x2, y2);
    case IDM_LINE:    return new LineShape(x1, y1, x2, y2);
    case IDM_RECT:    return new RectShape(x1, y1, x2, y2);
    case IDM_ELLIPSE: return new EllipseShape(x1, y1, x2, y2);
    default:          return nullptr;
    }
}

static void DrawAllShapes(HDC hdc)
{
    for (int i = 0; i < shapeCount; i++)
    {
        if (pcshape[i])
            pcshape[i]->Show(hdc);
    }
}

static void DrawRubberBand(HDC hdc, Shape* shape)
{
    // Variant: solid black line (16 mod 4 = 0)
    int oldROP = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    shape->Show(hdc);

    SelectObject(hdc, hOldPen);
    SetROP2(hdc, oldROP);
    DeleteObject(hPen);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetMenuCheckmarks(hWnd, currentType);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_POINT:
        case IDM_LINE:
        case IDM_RECT:
        case IDM_ELLIPSE:
            currentType = wmId;
            SetMenuCheckmarks(hWnd, currentType);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT:
            MessageBox(hWnd,
                _T("Lab 2 — Graphics Editor\nStepanenko Denys, IM-051\nJ=16: static array, black rubber band, orange rect, white ellipse"),
                _T("About"), MB_OK | MB_ICONINFORMATION);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        isDrawing = true;
        pTempShape = CreateShape(currentType, x, y, x, y);
    }
    break;

    case WM_MOUSEMOVE:
    {
        if (isDrawing && pTempShape)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            pTempShape->OnMouseMove(x, y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_LBUTTONUP:
    {
        if (isDrawing && pTempShape)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            pTempShape->OnMouseMove(x, y);

            if (shapeCount < N)
            {
                // Recreate shape with final coords
                int x1, y1, x2, y2;
                pTempShape->GetCoords(x1, y1, x2, y2);
                pcshape[shapeCount] = CreateShape(currentType, x1, y1, x2, y2);
                shapeCount++;
            }

            delete pTempShape;
            pTempShape = NULL;
            isDrawing = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        DrawAllShapes(hdc);

        if (isDrawing && pTempShape)
            DrawRubberBand(hdc, pTempShape);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        for (int i = 0; i < shapeCount; i++)
            delete pcshape[i];
        if (pTempShape) delete pTempShape;
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
// cd "F:\VSC projects\OOP_Labs\labs\lab2\code"
// msbuild Lab2.sln /p:Configuration=Debug /p:Platform=x64
// .\x64\Debug\Lab2.exe
