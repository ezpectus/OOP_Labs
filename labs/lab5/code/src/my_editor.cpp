// my_editor.cpp — MyEditor classic Singleton implementation
#include "my_editor.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"
#include "linecircles.h"
#include "cube.h"
#include "triangle.h"
#include "resource.h"
#include <stdio.h>
#include <tchar.h>
#include <commdlg.h>

MyEditor* MyEditor::p_instance = nullptr;

MyEditor::MyEditor() : shapeCount(0), currentType(IDM_POINT), isDrawing(false), pTempShape(NULL), pSelected(NULL)
{
    for (int i = 0; i < N; i++) pcshape[i] = NULL;
}

MyEditor::~MyEditor()
{
    for (int i = 0; i < shapeCount; i++) delete pcshape[i];
    if (pTempShape) delete pTempShape;
}

MyEditor* MyEditor::getInstance()
{
    if (!p_instance)
        p_instance = new MyEditor();
    return p_instance;
}

Shape* MyEditor::CreateShape(int type, int x1, int y1, int x2, int y2)
{
    switch (type)
    {
    case IDM_POINT:    return new PointShape(x1, y1, x2, y2);
    case IDM_LINE:     return new LineShape(x1, y1, x2, y2);
    case IDM_RECT:     return new RectShape(x1, y1, x2, y2);
    case IDM_ELLIPSE:  return new EllipseShape(x1, y1, x2, y2);
    case IDM_LINECIRC: return new LineWithCircles(x1, y1, x2, y2);
    case IDM_CUBE:     return new CubeWireframe(x1, y1, x2, y2);
    case IDM_TRIANGLE: return new TriangleShape(x1, y1, x2, y2);
    default:           return nullptr;
    }
}

int MyEditor::HitTestIndex(int x, int y)
{
    // topmost first: iterate from the end (later = drawn on top)
    for (int i = shapeCount - 1; i >= 0; i--)
        if (pcshape[i] && pcshape[i]->HitTest(x, y))
            return i;
    return -1;
}

int MyEditor::OnMouseDown(int x, int y)
{
    if (currentType == IDM_ERASER)
    {
        int idx = HitTestIndex(x, y);
        if (idx >= 0)
        {
            if (pcshape[idx] == pSelected) pSelected = NULL;
            delete pcshape[idx];
            for (int i = idx; i < shapeCount - 1; i++)
                pcshape[i] = pcshape[i + 1];
            pcshape[--shapeCount] = NULL;
        }
        return idx;   // erased index or -1 (caller syncs the table)
    }
    if (currentType == IDM_SELECT)
    {
        int idx = HitTestIndex(x, y);
        pSelected = (idx >= 0) ? pcshape[idx] : NULL;
        return -1;
    }
    isDrawing = true;
    pTempShape = CreateShape(currentType, x, y, x, y);
    if (pTempShape)
    {
        pTempShape->SetPenStyle(PS_DASH);        // rubber-band preview style
        pTempShape->SetPenColor(RGB(0, 0, 255)); // blue like lab3
        pTempShape->ClearFill();
    }
    return -1;
}

void MyEditor::OnMouseMove(int x, int y, HWND hWnd)
{
    if (isDrawing && pTempShape)
    {
        pTempShape->OnMouseMove(x, y);
        InvalidateRect(hWnd, NULL, FALSE);
    }
}

void MyEditor::OnMouseUp(int x, int y)
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
    }
}

void MyEditor::OnPaint(HDC hdc)
{
    for (int i = 0; i < shapeCount; i++)
        if (pcshape[i]) pcshape[i]->Show(hdc);

    if (isDrawing && pTempShape)
    {
        int oldROP = SetROP2(hdc, R2_NOTXORPEN);
        HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        pTempShape->Show(hdc);
        SelectObject(hdc, hOldPen);
        SetROP2(hdc, oldROP);
        DeleteObject(hPen);
    }

    // red dashed frame around the selected shape (Select tool)
    if (pSelected)
    {
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
}

void MyEditor::PickFillColor(HWND hWnd)
{
    if (!pSelected)
    {
        MessageBox(hWnd,
            _T("Select an object with the Select tool first"),
            _T("Fill color"), MB_OK | MB_ICONINFORMATION);
        return;
    }
    static COLORREF custom[16] = { 0 };
    CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner   = hWnd;
    cc.lpCustColors = custom;
    cc.rgbResult   = pSelected->GetFillColor();
    cc.Flags       = CC_FULLOPEN | CC_RGBINIT;
    if (ChooseColor(&cc))
        pSelected->SetFillColor(cc.rgbResult);
}

void MyEditor::PickPenColor(HWND hWnd)
{
    if (!pSelected)
    {
        MessageBox(hWnd,
            _T("Select an object with the Select tool first"),
            _T("Pen color"), MB_OK | MB_ICONINFORMATION);
        return;
    }
    static COLORREF custom[16] = { 0 };
    CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize  = sizeof(cc);
    cc.hwndOwner    = hWnd;
    cc.lpCustColors = custom;
    cc.Flags        = CC_FULLOPEN;
    if (ChooseColor(&cc))
        pSelected->SetPenColor(cc.rgbResult);
}

void MyEditor::ClearAll()
{
    for (int i = 0; i < shapeCount; i++)
    {
        delete pcshape[i];
        pcshape[i] = NULL;
    }
    shapeCount = 0;
    pSelected  = NULL;
}

int MyEditor::DeleteSelected()
{
    if (!pSelected) return -1;
    for (int i = 0; i < shapeCount; i++)
    {
        if (pcshape[i] == pSelected)
        {
            int removed = i;
            delete pcshape[i];
            for (int j = i; j < shapeCount - 1; j++)
                pcshape[j] = pcshape[j + 1];
            pcshape[--shapeCount] = NULL;
            pSelected = NULL;
            return removed;
        }
    }
    pSelected = NULL;
    return -1;
}

void MyEditor::SaveToFile(const wchar_t* filename)
{
    FILE* fout;
    if (_wfopen_s(&fout, filename, L"wt") != 0 || !fout) return;

    for (int i = 0; i < shapeCount; i++)
    {
        const wchar_t* name = pcshape[i]->GetName();

        int x1, y1, x2, y2;
        pcshape[i]->GetCoords(x1, y1, x2, y2);
        fwprintf(fout, L"%s\t%d\t%d\t%d\t%d\n", name, x1, y1, x2, y2);
    }
    fclose(fout);
}
