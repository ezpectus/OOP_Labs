// point.cpp - Point shape implementation
#include "point.h"

void PointShape::Show(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(m_fillColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(m_penStyle, 1, m_penColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    Ellipse(hdc, x1 - 3, y1 - 3, x1 + 3, y1 + 3);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
