// lineoo.h - Line with circles at both ends (multiple inheritance)
#pragma once
#include "line.h"
#include "ellipse.h"

// MI: the segment comes from LineShape, the end circles from EllipseShape.
// Both parents have "virtual public Shape" -> a single shared Shape
// subobject holds x1..y2 and the style fields (diamond problem solved).
class LineOOShape : public LineShape, public EllipseShape
{
public:
    LineOOShape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : Shape(x1, y1, x2, y2),            // most-derived inits virtual base
          LineShape(x1, y1, x2, y2),
          EllipseShape(x1, y1, x2, y2) {}

    void Show(HDC hdc) override;
};
