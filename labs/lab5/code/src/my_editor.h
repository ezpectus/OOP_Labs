// my_editor.h — MyEditor as classic Singleton (J=16 even)
#pragma once
#include <windows.h>
#include "shape.h"

#define N 117

class MyEditor
{
private:
    static MyEditor* p_instance;
    Shape* pcshape[N];
    int shapeCount;
    int currentType;
    bool isDrawing;
    Shape* pTempShape;
    Shape* pSelected;   // shape picked by the Select tool (fill target)

    MyEditor();
    MyEditor(const MyEditor&) = delete;
    MyEditor& operator=(const MyEditor&) = delete;

    Shape* CreateShape(int type, int x1, int y1, int x2, int y2);
    int  HitTestIndex(int x, int y);   // topmost shape index or -1

public:
    ~MyEditor();
    static MyEditor* getInstance();

    int  OnMouseDown(int x, int y);    // returns erased index or -1
    void OnMouseMove(int x, int y, HWND hWnd);
    void OnMouseUp(int x, int y);
    void OnPaint(HDC hdc);
    void SelectShape(int type) { currentType = type; }
    int  GetCurrentType() const { return currentType; }
    int  GetCount() const { return shapeCount; }
    Shape* GetShape(int i) const { return (i >= 0 && i < shapeCount) ? pcshape[i] : NULL; }

    void PickFillColor(HWND hWnd);
    void SaveToFile(const wchar_t* filename);
};
