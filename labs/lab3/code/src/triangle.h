// triangle.h - Triangle shape (5th object type)
#pragma once
#include "shape.h"

class TriangleShape : public Shape
{
public:
    TriangleShape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : Shape(x1, y1, x2, y2)
    {
        m_hasFill = true;
        m_fillColor = RGB(255, 165, 0);   // orange
    }

    void Show(HDC hdc) override;
    bool HitTest(int x, int y) const override;
};
