// ellipse.cpp - Ellipse shape implementation
// Variant: black outline + light-green fill (14 mod 5 = 4 -> color fill, 14 mod 6 = 2 -> light-green)
// Input: from center to corner (14 mod 2 = 0)
#include "ellipse.h"
#include <stdlib.h>

void EllipseShape::Show(HDC hdc)
{
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, m_penColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // input: center -> corner
    int rx = abs(x2 - x1);
    int ry = abs(y2 - y1);

    Ellipse(hdc, x1 - rx, y1 - ry, x1 + rx, y1 + ry);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    if (m_hasFill) DeleteObject(hBrush);
    DeleteObject(hPen);
}

RECT EllipseShape::GetBounds() const
{
    int rx = abs(x2 - x1);
    int ry = abs(y2 - y1);
    RECT r = { x1 - rx, y1 - ry, x1 + rx, y1 + ry };
    return r;
}

bool EllipseShape::HitTest(int x, int y) const
{
    // normalized ellipse equation: (dx/rx)^2 + (dy/ry)^2 <= 1
    double rx = (double)abs(x2 - x1);
    double ry = (double)abs(y2 - y1);
    if (rx < 1.0) rx = 1.0;
    if (ry < 1.0) ry = 1.0;
    double dx = (x - x1) / rx;
    double dy = (y - y1) / ry;
    return dx * dx + dy * dy <= 1.0;
}
