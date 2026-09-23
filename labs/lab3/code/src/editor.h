// editor.h - editor object: message handlers as class members
#pragma once
#include <windows.h>
#include "shape.h"

#define N 114

class MyEditor
{
public:
    MyEditor();

    void OnCommand(HWND hWnd, WPARAM wParam);
    void OnInitMenuPopup(WPARAM wParam);
    void OnLButtonDown(HWND hWnd, int x, int y);
    void OnMouseMove(HWND hWnd, int x, int y);
    void OnLButtonUp(HWND hWnd, int x, int y);
    void OnPaint(HWND hWnd);
    void OnDestroy();

private:
    Shape* pcshape[N];
    int shapeCount;
    int currentType;
    bool isDrawing;
    Shape* pTempShape;

    Shape* CreateShape(int type, int x1, int y1, int x2, int y2);
    void DrawAllShapes(HDC hdc);
    void DrawRubberBand(HDC hdc, Shape* shape);
};
