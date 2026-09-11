// rect.cpp — Rectangle shape implementation
// Variant: black outline, no fill (13 mod 5 = 3)
// Input: from center to corner (13 mod 2 = 1)
#include "rect.h"

void RectShape::Show(HDC hdc)
{
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // input: center -> corner
    int left   = 2 * x1 - x2;
    int top    = 2 * y1 - y2;
    int right  = x2;
    int bottom = y2;

    // Normalize
    if (left > right)  { int t = left;   left = right;   right = t; }
    if (top > bottom)  { int t = top;    top = bottom;   bottom = t; }

    Rectangle(hdc, left, top, right, bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}
