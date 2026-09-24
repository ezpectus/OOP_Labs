// ellipse.h - Ellipse shape
#pragma once
#include "shape.h"

// virtual: shared Shape subobject for multiple-inheritance shapes
class EllipseShape : virtual public Shape
{
public:
    EllipseShape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : Shape(x1, y1, x2, y2)
    {
        m_hasFill = true;
        m_fillColor = RGB(144, 238, 144);   // light-green (as in Lab 3)
    }

    void Show(HDC hdc) override;
};
