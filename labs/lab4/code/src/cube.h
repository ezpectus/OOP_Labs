// cube.h — Cube wireframe: multiple inheritance from LineShape + RectShape
#pragma once
#include "line.h"
#include "rect.h"

class CubeWireframe : public LineShape, public RectShape
{
public:
    CubeWireframe(int x1=0, int y1=0, int x2=0, int y2=0)
        : LineShape(x1, y1, x2, y2), RectShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override
    {
        int lx1 = x1, ly1 = y1, lx2 = x2, ly2 = y2;

        // Draw front face rectangle using RectShape::Show
        RectShape front(lx1, ly1, lx2, ly2);
        front.Show(hdc);

        // Calculate offset for back face (isometric projection)
        int dx = (lx2 - lx1) / 4;
        int dy = (ly2 - ly1) / 4;

        // Draw back face rectangle
        RectShape back(lx1 + dx, ly1 - dy, lx2 + dx, ly2 - dy);
        back.Show(hdc);

        // Draw connecting lines using LineShape::Show
        LineShape edge1(lx1, ly1, lx1 + dx, ly1 - dy);
        edge1.Show(hdc);

        LineShape edge2(lx2, ly1, lx2 + dx, ly1 - dy);
        edge2.Show(hdc);

        LineShape edge3(lx2, ly2, lx2 + dx, ly2 - dy);
        edge3.Show(hdc);

        LineShape edge4(lx1, ly2, lx1 + dx, ly2 - dy);
        edge4.Show(hdc);
    }

    void OnMouseDown(int x, int y) override
    {
        RectShape::OnMouseDown(x, y);
    }

    void OnMouseMove(int x, int y) override
    {
        RectShape::OnMouseMove(x, y);
    }

    void GetCoords(int& a, int& b, int& c, int& d) const
    {
        RectShape::GetCoords(a, b, c, d);
    }
};
