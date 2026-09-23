// shape.cpp - base class implementation
#include "shape.h"

RECT Shape::GetBounds() const
{
    RECT r;
    r.left   = (x1 < x2) ? x1 : x2;
    r.right  = (x1 < x2) ? x2 : x1;
    r.top    = (y1 < y2) ? y1 : y2;
    r.bottom = (y1 < y2) ? y2 : y1;
    return r;
}

bool Shape::HitTest(int x, int y) const
{
    RECT r = GetBounds();
    return x >= r.left && x <= r.right && y >= r.top && y <= r.bottom;
}
