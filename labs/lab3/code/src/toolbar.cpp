// toolbar.cpp - Toolbar creation, icon bitmaps, tooltip text
#include "toolbar.h"
#include "resource.h"
#include <commctrl.h>
#include <tchar.h>

#define BTN_COUNT 4

LPCTSTR ToolTipText(int cmdId)
{
    switch (cmdId)
    {
    case IDM_POINT:   return _T("Точка");
    case IDM_LINE:    return _T("Лінія");
    case IDM_RECT:    return _T("Прямокутник");
    case IDM_ELLIPSE: return _T("Еліпс");
    default:          return _T("");
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

    const int ids[BTN_COUNT] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE };
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
