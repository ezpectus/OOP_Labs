// shape.h — abstract base class
#pragma once
#include <windows.h>

class Shape
{
protected:
    int x1, y1, x2, y2;
public:
    Shape(int x1=0, int y1=0, int x2=0, int y2=0) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    virtual ~Shape() {}
    virtual void Show(HDC hdc) = 0;
    virtual void OnMouseDown(int x, int y) { x1=x; y1=y; x2=x; y2=y; }
    virtual void OnMouseMove(int x, int y) { x2=x; y2=y; }
    void GetCoords(int& a, int& b, int& c, int& d) const { a=x1; b=y1; c=x2; d=y2; }
};
