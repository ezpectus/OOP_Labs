// rect.h
#pragma once
#include "shape.h"
class RectShape : virtual public Shape {
public:
    RectShape(int x1=0, int y1=0, int x2=0, int y2=0) : Shape(x1,y1,x2,y2) {
        m_hasFill = true;
        m_fillColor = RGB(192,192,192);   // gray fill per variant
    }
    void Show(HDC hdc) override;
    void OnMouseDown(int x, int y) override { x1=x; y1=y; x2=x; y2=y; }
    void OnMouseMove(int x, int y) override { x2=x; y2=y; }
    RECT GetBounds() const override;      // x1,y1 = center -> real rect
    const wchar_t* GetName() const override { return L"Rectangle"; }
};
