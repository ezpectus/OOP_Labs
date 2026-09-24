// lineoo.cpp - line with circles: Show() delegates to both parent Shows
#include "lineoo.h"

#define CIRCLE_R 8

void LineOOShape::Show(HDC hdc)
{
    int ax1 = x1, ay1 = y1, ax2 = x2, ay2 = y2;   // save shared coords

    // the segment itself - reuse LineShape::Show (outline only)
    bool savedFill = m_hasFill;
    m_hasFill = false;
    LineShape::Show(hdc);
    m_hasFill = savedFill;

    // circles at both ends - reuse EllipseShape::Show (center -> corner)
    x1 = ax1; y1 = ay1; x2 = ax1 + CIRCLE_R; y2 = ay1 + CIRCLE_R;
    EllipseShape::Show(hdc);
    x1 = ax2; y1 = ay2; x2 = ax2 + CIRCLE_R; y2 = ay2 + CIRCLE_R;
    EllipseShape::Show(hdc);

    x1 = ax1; y1 = ay1; x2 = ax2; y2 = ay2;       // restore shared coords
}
