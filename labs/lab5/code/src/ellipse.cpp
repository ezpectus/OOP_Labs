// ellipse.cpp — Ellipse: two corners input, no fill by default
#include "ellipse.h"
#include <math.h>

void EllipseShape::Show(HDC hdc) {
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldB = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(m_penStyle, 1, m_penColor);
    HPEN hOldP = (HPEN)SelectObject(hdc, hPen);
    int left = (x1<x2)?x1:x2, right = (x1<x2)?x2:x1;
    int top = (y1<y2)?y1:y2, bottom = (y1<y2)?y2:y1;
    Ellipse(hdc, left, top, right, bottom);
    SelectObject(hdc, hOldP); SelectObject(hdc, hOldB);
    DeleteObject(hPen);
    if (m_hasFill) DeleteObject(hBrush);
}

bool EllipseShape::HitTest(int x, int y) const {
    // normalized ellipse equation over the bounding box:
    // ((x-cx)/rx)^2 + ((y-cy)/ry)^2 <= 1
    int left = (x1<x2)?x1:x2, right = (x1<x2)?x2:x1;
    int top = (y1<y2)?y1:y2, bottom = (y1<y2)?y2:y1;
    double cx = (left + right) / 2.0, cy = (top + bottom) / 2.0;
    double rx = (right - left) / 2.0, ry = (bottom - top) / 2.0;
    if (rx < 1.0) rx = 1.0;
    if (ry < 1.0) ry = 1.0;
    double dx = (x - cx) / rx, dy = (y - cy) / ry;
    return dx*dx + dy*dy <= 1.0;
}
