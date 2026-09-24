// cube.cpp - wireframe cube: faces via RectShape::Show, edges via LineShape::Show
#include "cube.h"

void CubeShape::Show(HDC hdc)
{
    int ax1 = x1, ay1 = y1, ax2 = x2, ay2 = y2;   // save shared coords

    // normalized front face
    int fx1 = (ax1 < ax2) ? ax1 : ax2;
    int fy1 = (ay1 < ay2) ? ay1 : ay2;
    int fx2 = (ax1 < ax2) ? ax2 : ax1;
    int fy2 = (ay1 < ay2) ? ay2 : ay1;

    // back face: shifted up-right by a third of the size
    int dx = (fx2 - fx1) / 3, dy = (fy2 - fy1) / 3;
    int bx1 = fx1 + dx, by1 = fy1 - dy;
    int bx2 = fx2 + dx, by2 = fy2 - dy;

    // front + back faces - reuse RectShape::Show (no fill -> wireframe)
    x1 = fx1; y1 = fy1; x2 = fx2; y2 = fy2;
    RectShape::Show(hdc);
    x1 = bx1; y1 = by1; x2 = bx2; y2 = by2;
    RectShape::Show(hdc);

    // 4 connecting edges - reuse LineShape::Show
    x1 = fx1; y1 = fy1; x2 = bx1; y2 = by1; LineShape::Show(hdc);
    x1 = fx2; y1 = fy1; x2 = bx2; y2 = by1; LineShape::Show(hdc);
    x1 = fx1; y1 = fy2; x2 = bx1; y2 = by2; LineShape::Show(hdc);
    x1 = fx2; y1 = fy2; x2 = bx2; y2 = by2; LineShape::Show(hdc);

    x1 = ax1; y1 = ay1; x2 = ax2; y2 = ay2;       // restore shared coords
}
