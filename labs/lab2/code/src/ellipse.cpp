// ellipse.cpp — Ellipse shape implementation
// Variant: black outline + yellow fill (13 mod 5 = 3 -> color fill, 13 mod 6 = 1 -> yellow)
// Input: by two corners of bounding rectangle (13 mod 2 = 1)
#include "ellipse.h"

void EllipseShape::Show(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int left   = (x1 < x2) ? x1 : x2;
    int right  = (x1 < x2) ? x2 : x1;
    int top    = (y1 < y2) ? y1 : y2;
    int bottom = (y1 < y2) ? y2 : y1;

    Ellipse(hdc, left, top, right, bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
