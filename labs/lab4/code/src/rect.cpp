// rect.cpp - Rectangle shape implementation
// Variant (as in Lab 3): black outline, no fill; input by two corners
#include "rect.h"

void RectShape::Show(HDC hdc)
{
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, m_penColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int left   = (x1 < x2) ? x1 : x2;
    int right  = (x1 < x2) ? x2 : x1;
    int top    = (y1 < y2) ? y1 : y2;
    int bottom = (y1 < y2) ? y2 : y1;

    Rectangle(hdc, left, top, right, bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    if (m_hasFill) DeleteObject(hBrush);
    DeleteObject(hPen);
}
