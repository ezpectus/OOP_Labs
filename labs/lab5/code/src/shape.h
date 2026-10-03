// shape.h — abstract base class
#pragma once
#include <windows.h>

class Shape
{
protected:
    int x1, y1, x2, y2;
    COLORREF m_fillColor;
    COLORREF m_penColor;
    int  m_penStyle;
    bool m_hasFill;

public:
    Shape(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
        : x1(x1), y1(y1), x2(x2), y2(y2),
          m_fillColor(RGB(0, 0, 0)), m_penColor(RGB(0, 0, 0)),
          m_penStyle(PS_SOLID), m_hasFill(false) {}

    virtual ~Shape() {}

    virtual void Show(HDC hdc) = 0;
    virtual void OnMouseDown(int x, int y) { x1 = x; y1 = y; x2 = x; y2 = y; }
    virtual void OnMouseMove(int x, int y) { x2 = x; y2 = y; }

    virtual bool HitTest(int x, int y) const;   // default: point in bounding box
    virtual RECT GetBounds() const;             // bounding rect (selection frame)
    virtual const wchar_t* GetName() const = 0; // row name for the table window

    void GetCoords(int& a, int& b, int& c, int& d) const { a = x1; b = y1; c = x2; d = y2; }
    void SetFillColor(COLORREF c) { m_fillColor = c; m_hasFill = true; }
    void SetPenColor(COLORREF c) { m_penColor = c; }
    void SetPenStyle(int style) { m_penStyle = style; }
    void ClearFill() { m_hasFill = false; }
    COLORREF GetFillColor() const { return m_fillColor; }
};
