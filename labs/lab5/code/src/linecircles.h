// linecircles.h — Line with circles: multiple inheritance from LineShape + EllipseShape
#pragma once
#include "line.h"
#include "ellipse.h"

class LineWithCircles : public LineShape, public EllipseShape
{
public:
    LineWithCircles(int x1=0, int y1=0, int x2=0, int y2=0)
        : LineShape(x1, y1, x2, y2), EllipseShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override
    {
        // Draw line using LineShape::Show
        LineShape::Show(hdc);

        // Draw circles at both endpoints using EllipseShape::Show
        // We need to call EllipseShape::Show with modified coords
        // Temporarily set ellipse coords to small circles at endpoints
        int lx1 = x1, ly1 = y1, lx2 = x2, ly2 = y2;

        // Circle at start point (x1, y1) with radius 8
        EllipseShape circle1(lx1 - 8, ly1 - 8, lx1 + 8, ly1 + 8);
        circle1.Show(hdc);

        // Circle at end point (x2, y2) with radius 8
        EllipseShape circle2(lx2 - 8, ly2 - 8, lx2 + 8, ly2 + 8);
        circle2.Show(hdc);
    }

    void OnMouseDown(int x, int y) override
    {
        LineShape::OnMouseDown(x, y);
    }

    void OnMouseMove(int x, int y) override
    {
        LineShape::OnMouseMove(x, y);
    }

    void GetCoords(int& a, int& b, int& c, int& d) const
    {
        LineShape::GetCoords(a, b, c, d);
    }
};
