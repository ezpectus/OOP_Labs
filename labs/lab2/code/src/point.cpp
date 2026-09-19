// point.cpp — Point shape implementation
#include "point.h"

void PointShape::Show(HDC hdc)
{
    SetPixel(hdc, x1, y1, RGB(0, 0, 0));
}
