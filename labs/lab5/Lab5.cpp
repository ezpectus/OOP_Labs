// Lab5.cpp — main file
// Lab 5: Multi-window UI, Singleton, table, file save
// Stepanenko Denys, IM-051, 2026
// Variant: J=16 (even) — classic Singleton

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"
#include "my_editor.h"
#include "my_table.h"

#pragma comment(lib, "comctl32.lib")

static TCHAR szWindowClass[] = _T("Lab5WindowClass");
static TCHAR szTitleBase[] = _T("Lab 5 — Stepanenko Denys, IM-051");

static MyTable table;
static HINSTANCE g_hInst = NULL;

static const TCHAR* ShapeNames[] = {
    _T("Point"), _T("Line"), _T("Rectangle"), _T("Ellipse"),
    _T("Line w/ Circles"), _T("Cube Wireframe")
};

static const wchar_t* GetShapeNameW(Shape* s)
{
    if (dynamic_cast<PointShape*>(s)) return L"Point";
    if (dynamic_cast<LineShape*>(s)) return L"Line";
    if (dynamic_cast<RectShape*>(s)) return L"Rect";
    if (dynamic_cast<EllipseShape*>(s)) return L"Ellipse";
    if (dynamic_cast<LineWithCircles*>(s)) return L"LineCirc";
    if (dynamic_cast<CubeWireframe*>(s)) return L"Cube";
    return L"Unknown";
}

static void UpdateTitle(HWND hWnd, int type)
{
    TCHAR buf[128];
    int idx = type - IDM_POINT;
    if (idx >= 0 && idx < 6)
        _stprintf_s(buf, 128, _T("%s — [%s]"), szTitleBase, ShapeNames[idx]);
    else
        _stprintf_s(buf, 128, _T("%s"), szTitleBase);
    SetWindowText(hWnd, buf);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hToolBar = NULL;

    switch (message)
    {
    case WM_CREATE:
    {
        g_hInst = ((LPCREATESTRUCT)lParam)->hInstance;

        hToolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
            0, 0, 0, 0, hWnd, (HMENU)IDR_TOOLBAR, g_hInst, NULL);

        SendMessage(hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

        TBADDBITMAP tbab;
        tbab.hInst = HINST_COMMCTRL;
        tbab.nID = IDB_STD_SMALL_COLOR;
        SendMessage(hToolBar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

        const int stdBtns[6] = { STD_FILE_NEW, STD_CUT, STD_COPY, STD_PASTE, STD_REDOW, STD_UNDO };
        TBBUTTON tbb[6];
        ZeroMemory(tbb, sizeof(tbb));
        for (int i = 0; i < 6; i++)
        {
            tbb[i].iBitmap = stdBtns[i];
            tbb[i].idCommand = IDM_POINT + i;
            tbb[i].fsState = TBSTATE_ENABLED;
            tbb[i].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_GROUP;
        }
        SendMessage(hToolBar, TB_ADDBUTTONS, 6, (LPARAM)tbb);

        MyEditor::getInstance()->SelectShape(IDM_POINT);
        UpdateTitle(hWnd, IDM_POINT);
    }
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
        case IDM_LINECIRC:
        case IDM_CUBE:
            MyEditor::getInstance()->SelectShape(wmId);
            UpdateTitle(hWnd, wmId);
            break;
        case IDM_VIEW_TABLE:
            table.Activate(hWnd, g_hInst);
            break;
        case IDM_FILE_SAVE:
            MyEditor::getInstance()->SaveToFile(L"shapes.txt");
            MessageBox(hWnd, _T("Saved to shapes.txt"), _T("File"), MB_OK);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT:
            MessageBox(hWnd,
                _T("Lab 5 — Multi-window UI\nStepanenko Denys, IM-051\nJ=16: classic Singleton, independent my_table module"),
                _T("About"), MB_OK | MB_ICONINFORMATION);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_NOTIFY:
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->code == TTN_NEEDTEXT)
        {
            LPNMTTDISPINFO pttt = (LPNMTTDISPINFO)lParam;
            int idx = (int)pnmh->idFrom - IDM_POINT;
            if (idx >= 0 && idx < 6)
                pttt->lpszText = (LPTSTR)ShapeNames[idx];
            return 0;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;

    case WM_LBUTTONDOWN:
        MyEditor::getInstance()->OnMouseDown(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_MOUSEMOVE:
        MyEditor::getInstance()->OnMouseMove(LOWORD(lParam), HIWORD(lParam), hWnd);
        break;

    case WM_LBUTTONUP:
    {
        MyEditor* editor = MyEditor::getInstance();
        int prevCount = editor->GetCount();
        editor->OnMouseUp(LOWORD(lParam), HIWORD(lParam));

        // Add to table if new shape was added
        if (editor->GetCount() > prevCount && table.IsActive())
        {
            Shape* s = editor->GetShape(editor->GetCount() - 1);
            if (s)
            {
                const wchar_t* name = GetShapeNameW(s);
                int x1, y1, x2, y2;
                s->GetCoords(x1, y1, x2, y2);
                table.Add(name, x1, y1, x2, y2);
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        MyEditor::getInstance()->OnPaint(hdc);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:
        SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
        break;

    case WM_DESTROY:
        table.Close();
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

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);

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
        MessageBox(NULL, _T("Failed to register window class"), _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateWindow(
        szWindowClass, szTitleBase,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL);

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
        if (!IsDialogMessage(table.GetHwnd(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

// LAUNCH (PowerShell):
// cd "F:\VSC projects\OOP_Labs\labs\lab5"
// msbuild Lab5.sln /p:Configuration=Debug /p:Platform=x64
// .\x64\Debug\Lab5.exe
