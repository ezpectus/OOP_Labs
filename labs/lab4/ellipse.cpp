// ellipse.cpp — Ellipse: two corners (J=17 mod 2=1), no fill (mod 5=2)
#include "ellipse.h"
void EllipseShape::Show(HDC hdc) {
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldB = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HPEN hOldP = (HPEN)SelectObject(hdc, hPen);
    int left = (x1<x2)?x1:x2, right = (x1<x2)?x2:x1;
    int top = (y1<y2)?y1:y2, bottom = (y1<y2)?y2:y1;
    Ellipse(hdc, left, top, right, bottom);
    SelectObject(hdc, hOldP); SelectObject(hdc, hOldB);
    DeleteObject(hPen);
}
