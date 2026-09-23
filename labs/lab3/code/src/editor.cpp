// editor.cpp - MyEditor implementation
// Variant: J=14, static array N=114, solid blue rubber band,
//   rect: 2 corners + no fill, ellipse: center->corner + light-green fill
#include "editor.h"
#include <commctrl.h>
#include <commdlg.h>
#include "resource.h"
#include "toolbar.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"
#include "triangle.h"
#include <tchar.h>

MyEditor::MyEditor()
{
    memset(pcshape, 0, sizeof(pcshape));
    shapeCount = 0;
    currentType = IDM_POINT;
    isDrawing = false;
    pTempShape = NULL;
    pSelected = NULL;
    hToolbar = NULL;
}

void MyEditor::OnCreate(HWND hWnd)
{
    hToolbar = CreateEditorToolbar(hWnd,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
}

void MyEditor::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR nmh = (LPNMHDR)lParam;
    if (nmh->code == TTN_NEEDTEXT)
    {
        LPTOOLTIPTEXT ttt = (LPTOOLTIPTEXT)lParam;
        ttt->lpszText = (LPTSTR)ToolTipText((int)nmh->idFrom);
    }
}

Shape* MyEditor::CreateShape(int type, int x1, int y1, int x2, int y2)
{
    switch (type)
    {
    case IDM_POINT:   return new PointShape(x1, y1, x2, y2);
    case IDM_LINE:    return new LineShape(x1, y1, x2, y2);
    case IDM_RECT:    return new RectShape(x1, y1, x2, y2);
    case IDM_ELLIPSE: return new EllipseShape(x1, y1, x2, y2);
    case IDM_TRIANGLE:return new TriangleShape(x1, y1, x2, y2);
    default:          return nullptr;
    }
}

void MyEditor::DrawAllShapes(HDC hdc)
{
    for (int i = 0; i < shapeCount; i++)
    {
        if (pcshape[i])
            pcshape[i]->Show(hdc);
    }
}

void MyEditor::DrawRubberBand(HDC hdc, Shape* shape)
{
    // Variant: solid blue line (14 mod 4 = 2)
    int oldROP = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    shape->Show(hdc);

    SelectObject(hdc, hOldPen);
    SetROP2(hdc, oldROP);
    DeleteObject(hPen);
}

void MyEditor::OnCommand(HWND hWnd, WPARAM wParam)
{
    int wmId = LOWORD(wParam);
    switch (wmId)
    {
    case IDM_POINT:
    case IDM_LINE:
    case IDM_RECT:
    case IDM_ELLIPSE:
    case IDM_TRIANGLE:
    case IDM_SELECT:
        currentType = wmId;
        // sync toolbar pressed state (CHECKGROUP unchecks the rest)
        SendMessage(hToolbar, TB_CHECKBUTTON, wmId, MAKELONG(TRUE, 0));
        break;
    case IDM_FILLCOLOR:
        PickFillColor(hWnd);
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    case IDM_ABOUT:
        MessageBox(hWnd,
            _T("Lab 3 - Graphics Editor\nStepanenko Denys, IM-051\nJ=14: static array, blue rubber band, no-fill rect, light-green ellipse"),
            _T("About"), MB_OK | MB_ICONINFORMATION);
        break;
    }
}

void MyEditor::OnInitMenuPopup(WPARAM wParam)
{
    // Type marker in Objects menu (14 mod 2 = 0)
    if (LOWORD(wParam) == 1)
    {
        CheckMenuRadioItem((HMENU)wParam, IDM_POINT, IDM_SELECT,
                           currentType, MF_BYCOMMAND);
    }
}

void MyEditor::OnLButtonDown(HWND hWnd, int x, int y)
{
    if (currentType == IDM_SELECT)
    {
        int idx = HitTestIndex(x, y);
        pSelected = (idx >= 0) ? pcshape[idx] : NULL;
        InvalidateRect(hWnd, NULL, FALSE);
        return;
    }

    isDrawing = true;
    pTempShape = CreateShape(currentType, x, y, x, y);
    SetCapture(hWnd);   // keep mouse msgs even if cursor leaves window
}

void MyEditor::OnMouseMove(HWND hWnd, int x, int y)
{
    if (isDrawing && pTempShape)
    {
        pTempShape->OnMouseMove(x, y);
        InvalidateRect(hWnd, NULL, FALSE);
    }
}

void MyEditor::OnLButtonUp(HWND hWnd, int x, int y)
{
    if (isDrawing && pTempShape)
    {
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
        ReleaseCapture();
        InvalidateRect(hWnd, NULL, FALSE);
    }
}

int MyEditor::HitTestIndex(int x, int y)
{
    // topmost (last drawn) shape first
    for (int i = shapeCount - 1; i >= 0; i--)
    {
        if (pcshape[i] && pcshape[i]->HitTest(x, y))
            return i;
    }
    return -1;
}

void MyEditor::DrawSelection(HDC hdc)
{
    if (!pSelected) return;

    RECT r = pSelected->GetBounds();
    InflateRect(&r, 4, 4);

    HPEN hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, r.left, r.top, r.right, r.bottom);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void MyEditor::PickFillColor(HWND hWnd)
{
    if (!pSelected)
    {
        MessageBox(hWnd,
            _T("Спочатку виберіть об'єкт інструментом Вибір"),
            _T("Колір заливки"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    static COLORREF custColors[16];
    CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize  = sizeof(cc);
    cc.hwndOwner    = hWnd;
    cc.lpCustColors = custColors;
    cc.rgbResult    = pSelected->GetFillColor();
    cc.Flags        = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc))
    {
        pSelected->SetFillColor(cc.rgbResult);
        InvalidateRect(hWnd, NULL, FALSE);
    }
}

void MyEditor::OnPaint(HWND hWnd)
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

    if (isDrawing && pTempShape)
        DrawRubberBand(hdcMem, pTempShape);

    DrawSelection(hdcMem);

    BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdc, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
    EndPaint(hWnd, &ps);
}

void MyEditor::OnDestroy()
{
    for (int i = 0; i < shapeCount; i++)
        delete pcshape[i];
    if (pTempShape) delete pTempShape;
}
