// ellipse.cpp - Ellipse shape implementation
// Variant (as in Lab 3): black outline + light-green fill; input center->corner
#include "ellipse.h"
#include <stdlib.h>

void EllipseShape::Show(HDC hdc)
{
    HBRUSH hBrush = m_hasFill ? CreateSolidBrush(m_fillColor)
                            : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(m_penStyle, 1, m_penColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // input: center -> corner
    int rx = abs(x2 - x1);
    int ry = abs(y2 - y1);

    Ellipse(hdc, x1 - rx, y1 - ry, x1 + rx, y1 + ry);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    if (m_hasFill) DeleteObject(hBrush);
    DeleteObject(hPen);
}
