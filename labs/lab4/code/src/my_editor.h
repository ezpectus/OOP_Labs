// my_editor.h — MyEditor class encapsulating editor logic
#pragma once
#include <windows.h>
#include "shape.h"

#define N 117

class MyEditor
{
private:
    Shape* pcshape[N];
    int shapeCount;
    int currentType;
    bool isDrawing;
    Shape* pTempShape;

    Shape* CreateShape(int type, int x1, int y1, int x2, int y2);

public:
    MyEditor();
    ~MyEditor();

    void OnMouseDown(int x, int y);
    void OnMouseMove(int x, int y, HWND hWnd);
    void OnMouseUp(int x, int y);
    void OnPaint(HDC hdc);
    void SelectShape(int type) { currentType = type; }
    int  GetCurrentType() const { return currentType; }
    bool IsDrawing() const { return isDrawing; }
};
