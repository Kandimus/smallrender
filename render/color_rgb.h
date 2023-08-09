#pragma once

#include <memory.h>
#include "defines.h"

namespace Render
{

class ColorRGB
{
public:
    ColorRGB() = default;
    ColorRGB(unsigned int c) { m_red = ((c >> 16) & 0xff) / 255.0; m_green = ((c >> 8) & 0xff) / 255.0; m_blue = (c & 0xff) / 255.0; }
    ColorRGB(REAL c) { m_red = m_green = m_blue = c; }
    ColorRGB(REAL r, REAL g, REAL b) { 	m_red = r; m_green = g; m_blue = b; }
    ColorRGB(const ColorRGB& c) { memcpy(m_color, c.data(), sizeof(REAL) * 3); }
    virtual~ColorRGB() = default;

    REAL& red() { return m_red; }
    REAL& green() { return m_green; }
    REAL& blue() { return m_blue; }

    REAL red() const { return m_red; }
    REAL green() const { return m_green; }
    REAL blue() const { return m_blue; }

    unsigned char redHex() const { return m_red * 255.0 + 0.5; }
    unsigned char greenHex() const { return m_green * 255.0 + 0.5; }
    unsigned char blueHex() const { return m_blue * 255.0 + 0.5; }

    REAL operator [](const int index) { return m_color[index]; }
    const REAL& operator [](const int index) const { return m_color[index]; }

    const REAL* data() const { return m_color; }

    void clamp() { 	for(int i = 0; i < 3; ++i) m_color[i] = std::min(std::max(m_color[i], REAL(0)), REAL(1)); }
    void scaleByMax();

    ColorRGB& operator =(const ColorRGB& c) { memcpy(m_color, c.data(), sizeof(REAL) * 3); return *this; }

    ColorRGB& operator +=(const ColorRGB& c) { m_red += c.red(); m_green += c.green(); m_blue += c.blue(); return *this; } //TODO оптимизировать
    ColorRGB& operator -=(const ColorRGB& c) { m_red -= c.red(); m_green -= c.green(); m_blue -= c.blue(); return *this; }
    ColorRGB& operator *=(const ColorRGB& c) { m_red *= c.red(); m_green *= c.green(); m_blue *= c.blue(); return *this; }
    ColorRGB& operator *=(REAL v) { m_red *= v; m_green *= v; m_blue *= v; return *this; }
    ColorRGB& operator /=(const ColorRGB& c)
    {
        if (c.red() == 0 || c.green() == 0 || c.blue() == 0) *this = Black;
        else { m_red /= c.red(); m_green /= c.green(); m_blue /= c.blue(); }
        return *this;
    }
    ColorRGB& operator /=(REAL v)
    {
        if (v == 0) *this = Black;
        else {m_red *= v; m_green *= v; m_blue *= v; }
        return *this;
    }

    bool operator ==(const ColorRGB& c) const { return m_red == c.red() && m_green == c.green() && m_blue == c.blue(); }
    bool operator !=(const ColorRGB& c) const { return m_red != c.red() || m_green != c.green() || m_blue != c.blue(); }

    ColorRGB operator -() const { return ColorRGB(1 - m_red, 1 - m_green, 1 - m_blue); }

    ColorRGB operator +(const ColorRGB& c) const { return ColorRGB(m_red + c.red(), m_green + c.green(), m_blue + c.blue()); }
    ColorRGB operator -(const ColorRGB& c) const { return ColorRGB(m_red - c.red(), m_green - c.green(), m_blue - c.blue()); }
    ColorRGB operator *(const ColorRGB& c) const { return ColorRGB(m_red * c.red(), m_green * c.green(), m_blue * c.blue()); }
    ColorRGB operator *(REAL v) const { return ColorRGB(m_red * v, m_green * v, m_blue * v); }
    ColorRGB operator /(const ColorRGB& c) const
    {
        return (c.red() == 0 || c.green() == 0 || c.blue() == 0)
                   ? Black
                   : ColorRGB(m_red / c.red(), m_green / c.green(), m_blue / c.blue());
    }
    ColorRGB operator /(REAL v) const { return v == 0 ? Black : ColorRGB(m_red / v, m_green / v, m_blue / v) ; }

    friend ColorRGB operator *(REAL scalar, const ColorRGB &zColor);
    friend ColorRGB operator /(REAL scalar, const ColorRGB &zColor);

    void adjustContrast(REAL c) { for (int i = 0; i < 3; ++i) m_color[i] = 0.5 + c * (m_color[i] - 0.5); }
    void adjustSaturation(REAL s) { REAL grey = m_red * 0.2125 + m_green * 0.7154 + m_blue * 0.0721f; for(int i = 0; i < 3; ++i) m_color[i] = grey + s * (m_color[i] - grey); }

    void lerp(REAL t, const ColorRGB& c1, const ColorRGB& c2) { for(int i = 0; i < 3; ++i) m_color[i] = c1[i] + t * (c2[i] - c1[i]); }

private:
	union
	{
		struct
		{
            REAL m_red;
            REAL m_green;
            REAL m_blue;
		};
        REAL m_color[3];
	};

public:
    static const ColorRGB Aquamarine;
    static const ColorRGB Black;
    static const ColorRGB Blue;
    static const ColorRGB BlueViolet;
    static const ColorRGB Brown;
    static const ColorRGB CadetBlue;
    static const ColorRGB Coral;
    static const ColorRGB CornflowerBlue;
    static const ColorRGB Cyan;
    static const ColorRGB DarkGreen;
    static const ColorRGB DarkOliveGreen;
    static const ColorRGB DarkOrhid;
    static const ColorRGB DarkSlateBlue;
    static const ColorRGB DarkSlateGray;
    static const ColorRGB DarkSlateGrey;
    static const ColorRGB DarkTurquiose;
    static const ColorRGB DimGray;
    static const ColorRGB DimGrey;
    static const ColorRGB Firebrick;
    static const ColorRGB ForestGreen;
    static const ColorRGB Gold;
    static const ColorRGB Goldenrod;
    static const ColorRGB Gray;
    static const ColorRGB Green;
    static const ColorRGB GreenYellow;
    static const ColorRGB Grey;
    static const ColorRGB Grey25;
    static const ColorRGB IndianRed;
    static const ColorRGB Khaki;
    static const ColorRGB LightBlue;
    static const ColorRGB LightGray;
    static const ColorRGB LightGrey;
    static const ColorRGB LightSteelBlue;
    static const ColorRGB LimeGreen;
    static const ColorRGB Magenta;
    static const ColorRGB Maroon;
    static const ColorRGB MediumAquamarine;
    static const ColorRGB MediumBlue;
    static const ColorRGB MediumForestGreen;
    static const ColorRGB MediumGoldenrod;
    static const ColorRGB MediumOrhid;
    static const ColorRGB MediumSeaGreen;
    static const ColorRGB MediumSlateBlue;
    static const ColorRGB MediumSpringGreen;
    static const ColorRGB MediumTurquoise;
    static const ColorRGB MediumVioletRed;
    static const ColorRGB MidnightBlue;
    static const ColorRGB Navy;
    static const ColorRGB NavyBlue;
    static const ColorRGB Orange;
    static const ColorRGB OrangeRed;
    static const ColorRGB Orhid;
    static const ColorRGB PaleGreen;
    static const ColorRGB Pink;
    static const ColorRGB Plum;
    static const ColorRGB Red;
    static const ColorRGB Salmon;
    static const ColorRGB SeaGreen;
    static const ColorRGB Sienna;
    static const ColorRGB SkyBlue;
    static const ColorRGB SlateBlue;
    static const ColorRGB SpringGreen;
    static const ColorRGB SteelBlue;
    static const ColorRGB Tan;
    static const ColorRGB Thistle;
    static const ColorRGB Turquoise;
    static const ColorRGB Violet;
    static const ColorRGB VioletRed;
    static const ColorRGB Wheat;
    static const ColorRGB White;
    static const ColorRGB Yellow;
};


inline ColorRGB operator *(REAL v, const ColorRGB& c)
{
    return ColorRGB(v * c.red(), v * c.green(), v * c.blue());
}

inline ColorRGB operator /(REAL v, const ColorRGB& c)
{
    if (c.red() == 0 || c.green() == 0 || c.blue() == 0)
    {
        return ColorRGB::Black;
    }

    return ColorRGB(v / c.red(), v / c.green(), v / c.blue());
}


//namespace Render
}
