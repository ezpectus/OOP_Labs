// triangle.cpp - Triangle shape implementation
// Input: by two corners of bounding rect; fill: orange by default
#include "triangle.h"

void TriangleShape::Show(HDC hdc)
{
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int left   = (x1 < x2) ? x1 : x2;
    int right  = (x1 < x2) ? x2 : x1;
    int top    = (y1 < y2) ? y1 : y2;
    int bottom = (y1 < y2) ? y2 : y1;

    // apex at top-center, base corners at bottom
    POINT pts[3] = {
        { (left + right) / 2, top },
        { left, bottom },
        { right, bottom }
    };
    Polygon(hdc, pts, 3);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    if (m_hasFill) DeleteObject(hBrush);
    DeleteObject(hPen);
}
