// rect.h - Rectangle shape
#pragma once
#include "shape.h"

// virtual: shared Shape subobject for multiple-inheritance shapes
class RectShape : virtual public Shape
{
public:
    RectShape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : Shape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override;
};
