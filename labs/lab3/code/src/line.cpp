// line.cpp - Line shape implementation
#include "line.h"
#include <math.h>

void LineShape::Show(HDC hdc)
{
    // line color = fill color if set, black otherwise
    HPEN hPen = CreatePen(PS_SOLID, 1, m_hasFill ? m_fillColor : RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

bool LineShape::HitTest(int x, int y) const
{
    // distance from point to segment <= 6
    double dx = x2 - x1, dy = y2 - y1;
    double len2 = dx * dx + dy * dy;
    if (len2 < 1.0) len2 = 1.0;
    double t = ((x - x1) * dx + (y - y1) * dy) / len2;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    double px = x1 + t * dx - x;
    double py = y1 + t * dy - y;
    return px * px + py * py <= 36.0;
}
