// point.cpp — Point shape
#include "point.h"

void PointShape::Show(HDC hdc) {
    SetPixel(hdc, x1, y1, m_penColor);
}

bool PointShape::HitTest(int x, int y) const {
    // tolerance ~6px so a single pixel can be clicked
    return abs(x - x1) <= 6 && abs(y - y1) <= 6;
}

RECT PointShape::GetBounds() const {
    RECT r = { x1 - 3, y1 - 3, x1 + 3, y1 + 3 };
    return r;
}
