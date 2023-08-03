#pragma once

#include "color_rgb.h"

namespace Render
{

class ColorARGB
{
public:
    ColorARGB() = default;
    ColorARGB(unsigned int c) : m_rgb(c) { from8888(c); }
    ColorARGB(REAL c) : m_rgb(c) { m_alpha = REAL(1); }
    ColorARGB(REAL r, REAL g, REAL b) : m_rgb(r, g, b) { m_alpha = REAL(1); }
    ColorARGB(REAL a, REAL r, REAL g, REAL b) : m_rgb(r, g, b) { m_alpha = a; }
    ColorARGB(const ColorRGB& c) : m_rgb(c) { m_alpha = REAL(1); }
    ColorARGB(REAL a, const ColorRGB& color) : m_rgb(color) { m_alpha = a; }
    ColorARGB(const ColorARGB& c) { m_alpha = c.alpha(); m_rgb = c.color(); }
    virtual ~ColorARGB() = default;

    REAL& alpha() { return m_alpha; }
    REAL& red() { return m_rgb.red(); }
    REAL& green() { return m_rgb.green(); }
    REAL& blue() { return m_rgb.blue(); }
    ColorRGB& color() { return m_rgb; }

    REAL alpha() const { return m_alpha; }
    REAL red() const { return m_rgb.red(); }
    REAL green() const { return m_rgb.green(); }
    REAL blue() const { return m_rgb.blue(); }
    const ColorRGB& color() const  { return m_rgb; }

    REAL operator [](const int index) { return index == 0 ? m_alpha: m_rgb[index - 1]; }
    const REAL& operator [](const int index) const { return index == 0 ? m_alpha: m_rgb[index - 1]; }

    operator ColorRGB() const { return m_rgb; }

    void from4444(unsigned short c);
    void from1555(unsigned short c);
    void from565(unsigned short c);
    void from888(unsigned int c);
    void from8888(unsigned int c);

    unsigned short to4444() const;
    unsigned short to1555() const;
    unsigned short to565() const;
    unsigned int to888() const;
    unsigned int to8888() const;

    void clamp() { m_alpha = std::min(std::max(m_alpha, REAL(0)), REAL(1)); m_rgb.clamp(); }
    void scaleByMax();

    ColorARGB& operator =(const ColorARGB& c) { m_alpha = c.alpha(); m_rgb = c.color(); return *this; }

    ColorARGB& operator +=(const ColorARGB& c) { m_alpha += c.alpha(); m_rgb += c.color(); return *this; }
    ColorARGB& operator -=(const ColorARGB& c) { m_alpha -= c.alpha(); m_rgb -= c.color(); return *this; }
    ColorARGB& operator *=(const ColorARGB& c) { m_alpha *= c.alpha(); m_rgb *= c.color(); return *this; }
    ColorARGB& operator *=(REAL v) { m_alpha *= v; m_rgb *= v; return *this; }
    ColorARGB& operator /=(const ColorARGB& c)
    {
        if (c.alpha() == 0 || c.red() == 0 || c.green() == 0 || c.blue() == 0) *this = ColorARGB(REAL(0));
        else { m_alpha /= c.alpha(); m_rgb /= c; }
        return *this;
    }
    ColorARGB& operator /=(REAL v)
    {
        if (v == 0 ) *this = ColorARGB(REAL(0));
        else { m_alpha /= v; m_rgb /= v; }
        return *this;
    }

    bool operator ==(const ColorARGB& c) const { return m_alpha == c.alpha() && m_rgb == c.color(); }
    bool operator !=(const ColorARGB& c) const { return m_alpha != c.alpha() || m_rgb != c.color(); }

    ColorARGB operator -() const { return ColorARGB(1.0 - m_alpha, -m_rgb); }

    ColorARGB operator +(const ColorARGB& c) const { return ColorARGB(m_alpha + c.alpha(), m_rgb + c.color()); }
    ColorARGB operator -(const ColorARGB& c) const { return ColorARGB(m_alpha - c.alpha(), m_rgb - c.color()); }
    ColorARGB operator *(const ColorARGB& c) const { return ColorARGB(m_alpha * c.alpha(), m_rgb * c.color()); }
    ColorARGB operator *(REAL v) const  { return ColorARGB(m_alpha + v, m_rgb + v); }
    ColorARGB operator /(const ColorARGB& c) const
    {
        return (c.alpha() == 0 || c.red() == 0 || c.green() == 0 || c.blue() == 0)
            ? ColorARGB(REAL(0))
            : ColorARGB(m_alpha / c.alpha(), m_rgb / c);
    }
    ColorARGB operator /(REAL v) const
    {
        return (v == 0)
            ? ColorARGB(REAL(0))
            : ColorARGB(m_alpha / v, m_rgb / v);
    }

	friend ColorARGB operator *(const float fScalar, const ColorARGB &zColor);
	friend ColorARGB operator /(const float fScalar, const ColorARGB &zColor);

    void adjustContrast(REAL c) { m_rgb.adjustContrast(c); }
    void adjustSaturation(REAL s) { m_rgb.adjustSaturation(s); }

    void lerp(REAL t, const ColorARGB& c1, const ColorARGB& c2) { m_alpha = c1.alpha() + t * (c2.alpha() - c1.alpha()); m_rgb.lerp(t, c1.color(), c2.color()); }

private:
    REAL m_alpha;
    ColorRGB m_rgb;

public:
    static const ColorARGB Aquamarine;
    static const ColorARGB Black;
    static const ColorARGB Blue;
    static const ColorARGB BlueViolet;
    static const ColorARGB Brown;
    static const ColorARGB CadetBlue;
    static const ColorARGB Coral;
    static const ColorARGB CornflowerBlue;
    static const ColorARGB Cyan;
    static const ColorARGB DarkGreen;
    static const ColorARGB DarkOliveGreen;
    static const ColorARGB DarkOrhid;
    static const ColorARGB DarkSlateBlue;
    static const ColorARGB DarkSlateGray;
    static const ColorARGB DarkSlateGrey;
    static const ColorARGB DarkTurquiose;
    static const ColorARGB DimGray;
    static const ColorARGB DimGrey;
    static const ColorARGB Firebrick;
    static const ColorARGB ForestGreen;
    static const ColorARGB Gold;
    static const ColorARGB Goldenrod;
    static const ColorARGB Gray;
    static const ColorARGB Green;
    static const ColorARGB GreenYellow;
    static const ColorARGB Grey;
    static const ColorARGB IndianRed;
    static const ColorARGB Khaki;
    static const ColorARGB LightBlue;
    static const ColorARGB LightGray;
    static const ColorARGB LightGrey;
    static const ColorARGB LightSteelBlue;
    static const ColorARGB LimeGreen;
    static const ColorARGB Magenta;
    static const ColorARGB Maroon;
    static const ColorARGB MediumAquamarine;
    static const ColorARGB MediumBlue;
    static const ColorARGB MediumForestGreen;
    static const ColorARGB MediumGoldenrod;
    static const ColorARGB MediumOrhid;
    static const ColorARGB MediumSeaGreen;
    static const ColorARGB MediumSlateBlue;
    static const ColorARGB MediumSpringGreen;
    static const ColorARGB MediumTurquoise;
    static const ColorARGB MediumVioletRed;
    static const ColorARGB MidnightBlue;
    static const ColorARGB Navy;
    static const ColorARGB NavyBlue;
    static const ColorARGB Orange;
    static const ColorARGB OrangeRed;
    static const ColorARGB Orhid;
    static const ColorARGB PaleGreen;
    static const ColorARGB Pink;
    static const ColorARGB Plum;
    static const ColorARGB Red;
    static const ColorARGB Salmon;
    static const ColorARGB SeaGreen;
    static const ColorARGB Sienna;
    static const ColorARGB SkyBlue;
    static const ColorARGB SlateBlue;
    static const ColorARGB SpringGreen;
    static const ColorARGB SteelBlue;
    static const ColorARGB Tan;
    static const ColorARGB Thistle;
    static const ColorARGB Turquoise;
    static const ColorARGB Violet;
    static const ColorARGB VioletRed;
    static const ColorARGB Wheat;
    static const ColorARGB White;
    static const ColorARGB Yellow;
};

inline unsigned short ColorARGB::to4444() const
{
    return static_cast<unsigned short>(
        ((static_cast<int>(m_alpha * 15) << 12) & 0xf000) +
        ((static_cast<int>(m_rgb.red() * 15) << 8) & 0xf00) +
        ((static_cast<int>(m_rgb.green() * 15) << 4) & 0xf0) +
        ( static_cast<int>(m_rgb.blue() * 15) & 0xf));
}

inline unsigned short ColorARGB::to1555() const
{
    return static_cast<unsigned short>(
        ((0 == m_alpha)? 0x8000 : 0) +
        ((static_cast<int>(m_rgb.red() * 31) << 10) & 0x7c00) +
        ((static_cast<int>(m_rgb.green() * 31) << 5) & 0x3e0) +
        ( static_cast<int>(m_rgb.blue() * 31) & 0x1f));
}

inline unsigned short ColorARGB::to565() const
{
    return static_cast<unsigned short>(
        ((static_cast<int>(m_rgb.red() * 31) << 11) & 0xf800) +
        ((static_cast<int>(m_rgb.green() * 63) << 5) & 0x7e0) +
        ( static_cast<int>(m_rgb.blue() * 31) & 0x1f));
}

inline unsigned int ColorARGB::to888() const
{
    return 0xff000000 +
           ((static_cast<int>(m_rgb.red() * 255) << 16) & 0xff0000) +
           ((static_cast<int>(m_rgb.green() * 255) << 8) & 0xff00) +
           ( static_cast<int>(m_rgb.blue() * 255) & 0xff);
}

inline unsigned int ColorARGB::to8888() const
{
    return ((static_cast<int>(m_alpha * 255) << 24) & 0xff000000) +
           ((static_cast<int>(m_rgb.red() * 255) << 16) & 0xff0000) +
           ((static_cast<int>(m_rgb.green() * 255) << 8) & 0xff00) +
           ( static_cast<int>(m_rgb.blue() * 255) & 0xff);
}

inline ColorARGB operator *(REAL v, const ColorARGB& c)
{
    return ColorARGB(v * c.alpha(), v * c.color());
}

inline ColorARGB operator /(REAL v, const ColorARGB& c)
{
    if (0 == c.alpha() || 0 == c.red() || 0 == c.green() || 0 == c.blue())
    {
        return ColorARGB::Black;
    }

    return ColorARGB(v / c.alpha(), v / c.color());
}


//namespace Render
}
