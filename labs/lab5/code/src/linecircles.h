// linecircles.h — Line with circles: multiple inheritance from LineShape + EllipseShape
#pragma once
#include "line.h"
#include "ellipse.h"

class LineWithCircles : public LineShape, public EllipseShape
{
public:
    LineWithCircles(int x1=0, int y1=0, int x2=0, int y2=0)
        : Shape(x1, y1, x2, y2), LineShape(x1, y1, x2, y2), EllipseShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override
    {
        // Draw line using LineShape::Show (shared virtual Shape fields)
        LineShape::Show(hdc);

        int lx1 = x1, ly1 = y1, lx2 = x2, ly2 = y2;

        // Circles at both endpoints (radius 8) — temp objects inherit our style
        EllipseShape circle1(lx1 - 8, ly1 - 8, lx1 + 8, ly1 + 8);
        circle1.SetPenStyle(m_penStyle);
        circle1.SetPenColor(m_penColor);
        if (m_hasFill) circle1.SetFillColor(m_fillColor);
        circle1.Show(hdc);

        EllipseShape circle2(lx2 - 8, ly2 - 8, lx2 + 8, ly2 + 8);
        circle2.SetPenStyle(m_penStyle);
        circle2.SetPenColor(m_penColor);
        if (m_hasFill) circle2.SetFillColor(m_fillColor);
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

    const wchar_t* GetName() const override { return L"LineCirc"; }

    // MI disambiguation: two parents override HitTest — pick Line's
    // (the line is the dominant body of this figure)
    bool HitTest(int x, int y) const override { return LineShape::HitTest(x, y); }
};
