// cube.h - wireframe cube (multiple inheritance)
#pragma once
#include "line.h"
#include "rect.h"

// MI: the four connecting edges come from LineShape,
// the front/back square faces come from RectShape.
// "virtual public Shape" in both parents -> one shared Shape subobject.
class CubeShape : public LineShape, public RectShape
{
public:
    CubeShape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : Shape(x1, y1, x2, y2),            // most-derived inits virtual base
          LineShape(x1, y1, x2, y2),
          RectShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override;
};
