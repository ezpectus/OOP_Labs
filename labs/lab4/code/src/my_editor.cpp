// my_editor.cpp — MyEditor implementation
#include "my_editor.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"
#include "linecircles.h"
#include "cube.h"
#include "resource.h"

MyEditor::MyEditor() : shapeCount(0), currentType(IDM_POINT), isDrawing(false), pTempShape(NULL)
{
    for (int i = 0; i < N; i++) pcshape[i] = NULL;
}

MyEditor::~MyEditor()
{
    for (int i = 0; i < shapeCount; i++) delete pcshape[i];
    if (pTempShape) delete pTempShape;
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
    default:           return nullptr;
    }
}

void MyEditor::OnMouseDown(int x, int y)
{
    isDrawing = true;
    pTempShape = CreateShape(currentType, x, y, x, y);
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
        // Dashed rubber band (Lab4 requirement)
        int oldROP = SetROP2(hdc, R2_NOTXORPEN);
        HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        pTempShape->Show(hdc);
        SelectObject(hdc, hOldPen);
        SetROP2(hdc, oldROP);
        DeleteObject(hPen);
    }
}
