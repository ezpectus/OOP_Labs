// rect.cpp — Rectangle: center input, gray fill — style rules inherited from Lab4
#include "rect.h"

void RectShape::Show(HDC hdc) {
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldB = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(m_penStyle, 1, m_penColor);
    HPEN hOldP = (HPEN)SelectObject(hdc, hPen);
    int left = 2*x1 - x2, top = 2*y1 - y2, right = x2, bottom = y2;
    if (left > right) { int t=left; left=right; right=t; }
    if (top > bottom) { int t=top; top=bottom; bottom=t; }
    Rectangle(hdc, left, top, right, bottom);
    SelectObject(hdc, hOldP); SelectObject(hdc, hOldB);
    DeleteObject(hPen);
    if (m_hasFill) DeleteObject(hBrush);
}

RECT RectShape::GetBounds() const {
    RECT r;
    r.left = 2*x1 - x2; r.top = 2*y1 - y2; r.right = x2; r.bottom = y2;
    if (r.left > r.right) { int t=r.left; r.left=r.right; r.right=t; }
    if (r.top > r.bottom) { int t=r.top; r.top=r.bottom; r.bottom=t; }
    return r;
}
