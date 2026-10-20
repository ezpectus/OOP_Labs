// point.cpp — Point shape
#include "point.h"

void PointShape::Show(HDC hdc) {
    // visible dot ~7px instead of a single pixel
    HPEN pen = CreatePen(PS_SOLID, 1, m_penColor);
    HBRUSH br = CreateSolidBrush(m_penColor);
    HGDIOBJ op = SelectObject(hdc, pen);
    HGDIOBJ ob = SelectObject(hdc, br);
    Ellipse(hdc, x1 - 3, y1 - 3, x1 + 3, y1 + 3);
    SelectObject(hdc, ob);
    SelectObject(hdc, op);
    DeleteObject(br);
    DeleteObject(pen);
}

bool PointShape::HitTest(int x, int y) const {
    // tolerance ~6px so a single pixel can be clicked
    return abs(x - x1) <= 6 && abs(y - y1) <= 6;
}

RECT PointShape::GetBounds() const {
    RECT r = { x1 - 3, y1 - 3, x1 + 3, y1 + 3 };
    return r;
}
