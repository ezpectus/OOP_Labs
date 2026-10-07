// ellipse.h
#pragma once
#include "shape.h"
class EllipseShape : virtual public Shape {
public:
    EllipseShape(int x1=0, int y1=0, int x2=0, int y2=0) : Shape(x1,y1,x2,y2) {}
    void Show(HDC hdc) override;
    bool HitTest(int x, int y) const override;
    const wchar_t* GetName() const override { return L"Ellipse"; }
};
