// rect.cpp — Rectangle: center input (J=17 mod 2=1), gray fill (mod 5=2, mod 6=5)
#include "rect.h"
void RectShape::Show(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(192,192,192));
    HBRUSH hOldB = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HPEN hOldP = (HPEN)SelectObject(hdc, hPen);
    int left = 2*x1 - x2, top = 2*y1 - y2, right = x2, bottom = y2;
    if (left > right) { int t=left; left=right; right=t; }
    if (top > bottom) { int t=top; top=bottom; bottom=t; }
    Rectangle(hdc, left, top, right, bottom);
    SelectObject(hdc, hOldP); SelectObject(hdc, hOldB);
    DeleteObject(hPen); DeleteObject(hBrush);
}
