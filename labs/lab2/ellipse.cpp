// ellipse.cpp — Ellipse shape implementation
// Variant: black outline + white fill (16 mod 5 = 1)
// Input: from center to corner (16 mod 2 = 0)
#include "ellipse.h"

void EllipseShape::OnMouseDown(int x, int y)
{
    // Center point
    x1 = x; y1 = y;
    x2 = x; y2 = y;
}

void EllipseShape::OnMouseMove(int x, int y)
{
    // x2,y2 is the corner; compute bounding rect from center
    x2 = x; y2 = y;
}

void EllipseShape::Show(HDC hdc)
{
    // White fill brush
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // Black outline pen
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Bounding rectangle from center (x1,y1) to corner (x2,y2)
    int left   = 2 * x1 - x2;
    int top    = 2 * y1 - y2;
    int right  = x2;
    int bottom = y2;

    // Normalize
    if (left > right) { int t = left; left = right; right = t; }
    if (top > bottom) { int t = top; top = bottom; bottom = t; }

    Ellipse(hdc, left, top, right, bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
