// cube.h — Cube wireframe: multiple inheritance from LineShape + RectShape
#pragma once
#include "line.h"
#include "rect.h"

class CubeWireframe : public LineShape, public RectShape
{
public:
    CubeWireframe(int x1=0, int y1=0, int x2=0, int y2=0)
        : Shape(x1, y1, x2, y2), LineShape(x1, y1, x2, y2), RectShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override
    {
        int lx1 = x1, ly1 = y1, lx2 = x2, ly2 = y2;
        int dx = (lx2 - lx1) / 4;
        int dy = (ly2 - ly1) / 4;

        // Helper: style a temp part with our pen settings
        // (wireframe: faces are NOT filled)
        RectShape front(lx1, ly1, lx2, ly2);
        front.SetPenStyle(m_penStyle); front.SetPenColor(m_penColor); front.ClearFill();
        front.Show(hdc);

        RectShape back(lx1 + dx, ly1 - dy, lx2 + dx, ly2 - dy);
        back.SetPenStyle(m_penStyle); back.SetPenColor(m_penColor); back.ClearFill();
        back.Show(hdc);

        LineShape edge1(lx1, ly1, lx1 + dx, ly1 - dy);
        edge1.SetPenStyle(m_penStyle); edge1.SetPenColor(m_penColor);
        edge1.Show(hdc);

        LineShape edge2(lx2, ly1, lx2 + dx, ly1 - dy);
        edge2.SetPenStyle(m_penStyle); edge2.SetPenColor(m_penColor);
        edge2.Show(hdc);

        LineShape edge3(lx2, ly2, lx2 + dx, ly2 - dy);
        edge3.SetPenStyle(m_penStyle); edge3.SetPenColor(m_penColor);
        edge3.Show(hdc);

        LineShape edge4(lx1, ly2, lx1 + dx, ly2 - dy);
        edge4.SetPenStyle(m_penStyle); edge4.SetPenColor(m_penColor);
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

    const wchar_t* GetName() const override { return L"Cube"; }

    // MI disambiguation + UX: use the rect's bounding-box hit test —
    // LineShape::HitTest would only accept clicks near the diagonal
    bool HitTest(int x, int y) const override { return RectShape::HitTest(x, y); }
};
