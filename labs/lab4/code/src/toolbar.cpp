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
    case IDM_LINEOO:   return _T("Лінія з кружечками");
    case IDM_CUBE:     return _T("Каркас куба");
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
    case IDM_LINEOO:
        // segment + a circle at each end
        MoveToEx(hdc, 6, 10, NULL);
        LineTo(hdc, 10, 6);
        Ellipse(hdc, 1, 9, 7, 15);
        Ellipse(hdc, 9, 1, 15, 7);
        break;
    case IDM_CUBE:
        // wireframe cube: front square, shifted back square, edges
        Rectangle(hdc, 1, 6, 9, 14);
        Rectangle(hdc, 6, 2, 14, 10);
        MoveToEx(hdc, 1, 6, NULL);  LineTo(hdc, 6, 2);
        MoveToEx(hdc, 9, 6, NULL);  LineTo(hdc, 14, 2);
        MoveToEx(hdc, 1, 14, NULL); LineTo(hdc, 6, 10);
        MoveToEx(hdc, 9, 14, NULL); LineTo(hdc, 14, 10);
        break;
    }

    SelectObject(hdc, hOld);
    DeleteObject(hPen);
}

HWND CreateEditorToolbar(HWND hParent, HINSTANCE hInst)
{
    // nBitmaps=0: we add images ourselves via TB_ADDBITMAP
    HWND hTB = CreateToolbarEx(hParent,
        WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_TOP,
        IDC_TOOLBAR, 0, NULL, 0, NULL, 0,
        24, 22, 16, 16, sizeof(TBBUTTON));
    SendMessage(hTB, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
    SendMessage(hTB, TB_SETBUTTONSIZE, 0, MAKELPARAM(24, 22));

    const int ids[BTN_COUNT] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE, IDM_LINEOO, IDM_CUBE };
    TBBUTTON tb[BTN_COUNT];

    HDC hdcScreen = GetDC(NULL);
    for (int i = 0; i < BTN_COUNT; i++)
    {
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HBITMAP hbm = CreateCompatibleBitmap(hdcScreen, 16, 16);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hbm);

        // button-face background (bitmap starts uninitialized)
        RECT rc = { 0, 0, 16, 16 };
        FillRect(hdcMem, &rc, (HBRUSH)(COLOR_BTNFACE + 1));
        DrawToolGlyph(hdcMem, ids[i]);
        SelectObject(hdcMem, hOldBmp);
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
