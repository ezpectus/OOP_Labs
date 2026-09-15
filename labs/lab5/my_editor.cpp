// my_editor.cpp — MyEditor classic Singleton implementation
#include "my_editor.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"
#include "linecircles.h"
#include "cube.h"
#include "resource.h"
#include <stdio.h>

MyEditor* MyEditor::p_instance = nullptr;

MyEditor::MyEditor() : shapeCount(0), currentType(IDM_POINT), isDrawing(false), pTempShape(NULL)
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
        InvalidateRect(hWnd, NULL, TRUE);
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
}

void MyEditor::SaveToFile(const wchar_t* filename)
{
    FILE* fout;
    if (_wfopen_s(&fout, filename, L"wt") != 0 || !fout) return;

    for (int i = 0; i < shapeCount; i++)
    {
        const wchar_t* name = L"Unknown";
        if (dynamic_cast<PointShape*>(pcshape[i])) name = L"Point";
        else if (dynamic_cast<LineShape*>(pcshape[i])) name = L"Line";
        else if (dynamic_cast<RectShape*>(pcshape[i])) name = L"Rect";
        else if (dynamic_cast<EllipseShape*>(pcshape[i])) name = L"Ellipse";
        else if (dynamic_cast<LineWithCircles*>(pcshape[i])) name = L"LineCirc";
        else if (dynamic_cast<CubeWireframe*>(pcshape[i])) name = L"Cube";

        int x1, y1, x2, y2;
        pcshape[i]->GetCoords(x1, y1, x2, y2);
        fwprintf(fout, L"%s\t%d\t%d\t%d\t%d\n", name, x1, y1, x2, y2);
    }
    fclose(fout);
}
