// line.cpp - Line shape implementation
#include "line.h"

void LineShape::Show(HDC hdc)
{
    // line color = fill color if set, black otherwise
    HPEN hPen = CreatePen(m_penStyle, 1, m_hasFill ? m_fillColor : m_penColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
