// Lab3.cpp — main file
// Lab 3: Graphics editor with Toolbar, tooltips, class-based handlers
// Stepanenko Denys, IM-051, 2026
// Variant: J=17, static array N=117, solid red rubber band,
//   rect: center input + gray fill, ellipse: 2 corners + no fill,
//   type indicator in window title

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"
#include "shape.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"

#pragma comment(lib, "comctl32.lib")

#define N 117

static TCHAR szWindowClass[] = _T("Lab3WindowClass");
static TCHAR szTitleBase[] = _T("Lab 3 — Stepanenko Denys, IM-051");

// Main window class — all message handlers are members
class MainWindow
{
private:
    HWND hWnd;
    HWND hToolBar;
    HINSTANCE hInst;
    int currentType;
    bool isDrawing;
    Shape* pTempShape;
    Shape* pcshape[N];
    int shapeCount;

    static const TCHAR* ShapeNames[];

    Shape* CreateShape(int type, int x1, int y1, int x2, int y2)
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

    void UpdateTitle()
    {
        TCHAR buf[128];
        swprintf(buf, 128, L"%s — [%s]", szTitleBase, ShapeNames[currentType - IDM_POINT]);
        SetWindowText(hWnd, buf);
    }

    void DrawAllShapes(HDC hdc)
    {
        for (int i = 0; i < shapeCount; i++)
            if (pcshape[i]) pcshape[i]->Show(hdc);
    }

    void DrawRubberBand(HDC hdc)
    {
        if (!pTempShape) return;
        // Variant: solid red line (17 mod 4 = 1)
        int oldROP = SetROP2(hdc, R2_NOTXORPEN);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        pTempShape->Show(hdc);
        SelectObject(hdc, hOldPen);
        SetROP2(hdc, oldROP);
        DeleteObject(hPen);
    }

    void SelectShape(int type)
    {
        currentType = type;
        UpdateTitle();
        // Sync toolbar
        SendMessage(hToolBar, TB_CHECKBUTTON, IDM_POINT,   type == IDM_POINT   ? TRUE : FALSE);
        SendMessage(hToolBar, TB_CHECKBUTTON, IDM_LINE,    type == IDM_LINE    ? TRUE : FALSE);
        SendMessage(hToolBar, TB_CHECKBUTTON, IDM_RECT,    type == IDM_RECT    ? TRUE : FALSE);
        SendMessage(hToolBar, TB_CHECKBUTTON, IDM_ELLIPSE, type == IDM_ELLIPSE ? TRUE : FALSE);
    }

public:
    MainWindow() : hWnd(NULL), hToolBar(NULL), hInst(NULL),
        currentType(IDM_POINT), isDrawing(false), pTempShape(NULL), shapeCount(0)
    {
        for (int i = 0; i < N; i++) pcshape[i] = NULL;
    }

    ~MainWindow()
    {
        for (int i = 0; i < shapeCount; i++) delete pcshape[i];
        if (pTempShape) delete pTempShape;
    }

    HWND GetHwnd() const { return hWnd; }

    void OnCreate(HWND hwnd, HINSTANCE hInstance)
    {
        hWnd = hwnd;
        hInst = hInstance;

        // Create toolbar
        hToolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
            0, 0, 0, 0, hWnd, (HMENU)IDR_TOOLBAR, hInst, NULL);

        SendMessage(hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

        // Load standard toolbar bitmaps (use system imagelist)
        TBADDBITMAP tbab;
        tbab.hInst = HINST_COMMCTRL;
        tbab.nID = IDB_STD_SMALL_COLOR;
        int idx0 = (int)SendMessage(hToolBar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

        // Add buttons
        TBBUTTON tbb[4];
        ZeroMemory(tbb, sizeof(tbb));
        const int stdBtns[4] = { 6, 3, 4, 5 };
        for (int i = 0; i < 4; i++)
        {
            tbb[i].iBitmap = stdBtns[i];
            tbb[i].idCommand = IDM_POINT + i;
            tbb[i].fsState = TBSTATE_ENABLED;
            tbb[i].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_GROUP;
            tbb[i].iString = 0;
        }
        SendMessage(hToolBar, TB_ADDBUTTONS, 4, (LPARAM)tbb);

        SelectShape(IDM_POINT);
    }

    LRESULT OnCommand(WPARAM wParam, LPARAM lParam)
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_POINT:
        case IDM_LINE:
        case IDM_RECT:
        case IDM_ELLIPSE:
            SelectShape(wmId);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT:
            MessageBox(hWnd,
                _T("Lab 3 — Graphics Editor with Toolbar\nStepanenko Denys, IM-051\nJ=17: red rubber band, gray rect, no-fill ellipse"),
                _T("About"), MB_OK | MB_ICONINFORMATION);
            break;
        default:
            return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
        }
        return 0;
    }

    LRESULT OnNotify(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->code == TTN_NEEDTEXT)
        {
            LPNMTTDISPINFO pttt = (LPNMTTDISPINFO)lParam;
            int btnId = (int)pnmh->idFrom;
            switch (btnId)
            {
            case IDM_POINT:   pttt->lpszText = (LPTSTR)_T("Point"); break;
            case IDM_LINE:    pttt->lpszText = (LPTSTR)_T("Line"); break;
            case IDM_RECT:    pttt->lpszText = (LPTSTR)_T("Rectangle"); break;
            case IDM_ELLIPSE: pttt->lpszText = (LPTSTR)_T("Ellipse"); break;
            }
            return 0;
        }
        return DefWindowProc(hWnd, WM_NOTIFY, wParam, lParam);
    }

    void OnLButtonDown(int x, int y)
    {
        isDrawing = true;
        pTempShape = CreateShape(currentType, x, y, x, y);
    }

    void OnMouseMove(int x, int y)
    {
        if (isDrawing && pTempShape)
        {
            pTempShape->OnMouseMove(x, y);
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }

    void OnLButtonUp(int x, int y)
    {
        if (isDrawing && pTempShape)
        {
            pTempShape->OnMouseMove(x, y);
            if (shapeCount < N)
            {
                int x1, y1, x2, y2;
                pTempShape->GetCoords(x1, y1, x2, y2);
                pcshape[shapeCount] = CreateShape(currentType, x1, y1, x2, y2);
                shapeCount++;
            }
            delete pTempShape;
            pTempShape = NULL;
            isDrawing = false;
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }

    void OnPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

        HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        FillRect(hdcMem, &rc, hBrush);
        DeleteObject(hBrush);

        DrawAllShapes(hdcMem);
        if (isDrawing && pTempShape) DrawRubberBand(hdcMem);

        BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
    }

    void OnDestroy()
    {
        PostQuitMessage(0);
    }

    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
            OnCreate(hWnd, ((LPCREATESTRUCT)lParam)->hInstance);
            return 0;
        case WM_COMMAND:     return OnCommand(wParam, lParam);
        case WM_NOTIFY:      return OnNotify(wParam, lParam);
        case WM_LBUTTONDOWN: OnLButtonDown(LOWORD(lParam), HIWORD(lParam)); return 0;
        case WM_MOUSEMOVE:   OnMouseMove(LOWORD(lParam), HIWORD(lParam)); return 0;
        case WM_LBUTTONUP:   OnLButtonUp(LOWORD(lParam), HIWORD(lParam)); return 0;
        case WM_PAINT:       OnPaint(); return 0;
        case WM_ERASEBKGND:  return 1;
        case WM_DESTROY:     OnDestroy(); return 0;
        case WM_SIZE:
            SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        MainWindow* pThis = NULL;
        if (message == WM_CREATE)
        {
            pThis = (MainWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->hWnd = hwnd;
        }
        else
        {
            pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis)
            return pThis->WndProc(message, wParam, lParam);
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
};

const TCHAR* MainWindow::ShapeNames[] = { _T("Point"), _T("Line"), _T("Rectangle"), _T("Ellipse") };

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_BAR_CLASSES | ICC_TAB_CLASSES;
    InitCommonControlsEx(&icc);

    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = MainWindow::StaticWndProc;
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
        MessageBox(NULL, _T("Failed to register window class"), _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    static MainWindow app;

    HWND hWnd = CreateWindow(
        szWindowClass, szTitleBase,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, &app);

    if (!hWnd)
    {
        MessageBox(NULL, _T("Failed to create window"), _T("Error"), MB_OK | MB_ICONERROR);
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
