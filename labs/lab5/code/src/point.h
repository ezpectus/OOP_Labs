// point.h
#pragma once
#include "shape.h"
class PointShape : virtual public Shape {
public:
    PointShape(int x1=0, int y1=0, int x2=0, int y2=0) : Shape(x1,y1,x2,y2) {}
    void Show(HDC hdc) override;
    void OnMouseDown(int x, int y) override { x1=x; y1=y; x2=x; y2=y; }
    void OnMouseMove(int, int) override {}
};
