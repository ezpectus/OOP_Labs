// line.h
#pragma once
#include "shape.h"
class LineShape : public Shape {
public:
    LineShape(int x1=0, int y1=0, int x2=0, int y2=0) : Shape(x1,y1,x2,y2) {}
    void Show(HDC hdc) override;
};
