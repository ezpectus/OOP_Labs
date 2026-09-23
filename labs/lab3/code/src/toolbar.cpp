// toolbar.cpp - Toolbar creation, icon bitmaps, tooltip text
#include "toolbar.h"
#include "resource.h"
#include <commctrl.h>
#include <tchar.h>

#define BTN_COUNT 6

LPCTSTR ToolTipText(int cmdId)
{
    switch (cmdId)
    {
    case IDM_POINT:    return _T("Точка");
    case IDM_LINE:     return _T("Лінія");
    case IDM_RECT:     return _T("Прямокутник");
    case IDM_ELLIPSE:  return _T("Еліпс");
    case IDM_TRIANGLE: return _T("Трикутник");
    case IDM_SELECT:   return _T("Вибір");
    default:           return _T("");
    }
}

// draws the tool glyph into a 16x16 DC
static void DrawToolGlyph(HDC hdc, int id)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOld = (HPEN)SelectObject(hdc, hPen);

    switch (id)
    {
    case IDM_POINT:
        Ellipse(hdc, 5, 5, 11, 11);
        break;
    case IDM_LINE:
        MoveToEx(hdc, 3, 13, NULL);
        LineTo(hdc, 13, 3);
        break;
    case IDM_RECT:
        Rectangle(hdc, 3, 4, 14, 12);
        break;
    case IDM_ELLIPSE:
        Ellipse(hdc, 3, 4, 14, 12);
        break;
    case IDM_TRIANGLE:
    {
        HBRUSH hb = CreateSolidBrush(RGB(255, 165, 0));
        HBRUSH hob = (HBRUSH)SelectObject(hdc, hb);
        POINT p[3] = { {8, 3}, {3, 13}, {13, 13} };
        Polygon(hdc, p, 3);
        SelectObject(hdc, hob);
        DeleteObject(hb);
        break;
    }
    case IDM_SELECT:
    {
        // arrow cursor glyph
        HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH hob = (HBRUSH)SelectObject(hdc, hb);
        POINT a[7] = { {4, 2}, {4, 12}, {7, 9}, {9, 14}, {11, 13}, {8, 8}, {12, 8} };
        Polygon(hdc, a, 7);
        SelectObject(hdc, hob);
        DeleteObject(hb);
        break;
    }
    }

    SelectObject(hdc, hOld);
    DeleteObject(hPen);
}

HWND CreateEditorToolbar(HWND hParent, HINSTANCE hInst)
{
    HWND hTB = CreateToolbarEx(hParent,
        WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_TOP,
        IDC_TOOLBAR, BTN_COUNT, NULL, 0, NULL, 0,
        0, 0, 16, 16, sizeof(TBBUTTON));

    const int ids[BTN_COUNT] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE, IDM_TRIANGLE, IDM_SELECT };
    TBBUTTON tb[BTN_COUNT];

    HDC hdcScreen = GetDC(NULL);
    for (int i = 0; i < BTN_COUNT; i++)
    {
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hbm = CreateCompatibleBitmap(hdcScreen, 16, 16);
        SelectObject(hdcMem, hbm);

        // button-face background (bitmap starts uninitialized)
        RECT rc = { 0, 0, 16, 16 };
        FillRect(hdcMem, &rc, (HBRUSH)(COLOR_BTNFACE + 1));
        DrawToolGlyph(hdcMem, ids[i]);
        DeleteDC(hdcMem);

        TBADDBITMAP ab;
        ab.hInst = NULL;
        ab.nID = (UINT_PTR)hbm;
        int idx = (int)SendMessage(hTB, TB_ADDBITMAP, 1, (LPARAM)&ab);

        tb[i].iBitmap   = idx;
        tb[i].idCommand = ids[i];
        tb[i].fsState   = TBSTATE_ENABLED;
        tb[i].fsStyle   = TBSTYLE_CHECKGROUP;
        tb[i].dwData    = 0;
        tb[i].iString   = 0;
    }
    ReleaseDC(NULL, hdcScreen);

    SendMessage(hTB, TB_ADDBUTTONS, BTN_COUNT, (LPARAM)tb);
    SendMessage(hTB, TB_AUTOSIZE, 0, 0);
    return hTB;
}
