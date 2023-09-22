#pragma once

#include <string>

#include "defines.h"

namespace Render
{

class Vector2
{
public:
    static Vector2 c0;
    static Vector2 c1;
    static Vector2 cX;
    static Vector2 cY;

    Vector2() {}
    Vector2(REAL v) { m_x = m_y = v; }
    Vector2(REAL x, REAL y)  { m_x = x; m_y = y; }
    Vector2(const Vector2& v) { m_x = v.x(); m_y = v.y(); }

    inline Vector2& operator =(REAL value) { m_x = m_y = value; return *this; }
    inline Vector2& operator =(const Vector2& v) { m_x = v.x(); m_y = v.y(); return *this; }

    inline Vector2& operator +=(const Vector2& v) { m_x += v.x(); m_y += v.y(); return *this; }
    inline Vector2& operator -=(const Vector2& v) { m_x -= v.x(); m_y -= v.y(); return *this; }
    inline Vector2& operator *=(REAL v) { m_x *= v; m_y *= v; return *this; }
    inline Vector2& operator *=(const Vector2& v) { m_x *= v.x(); m_y *= v.y(); return *this; }

    inline Vector2 operator -() const { return Vector2(-m_x, -m_y); }

    REAL length() const {return SQRT(squaredLength()); }
    inline REAL squaredLength() const { return m_x * m_x + m_y * m_y; }
    inline void normalize() { REAL l = length(); if(l != 0) (*this) *= 1 / l; else (*this) = 0; }
    inline void clip()
    {
        for (int i = 0; i < 2; ++i)
        {
            if (m_value[i] <= 0)
            {
                m_value[i] = 0;
            }
            else if (m_value[i] >= 1)
            {
                m_value[i] = 1;
            }
        }
    }

    inline Vector2 min(const Vector2& v) { return Vector2(std::min(m_x, v.x()), std::min(m_y, v.y())); }
    inline Vector2 max(const Vector2& v) { return Vector2(std::max(m_x, v.x()), std::max(m_y, v.y())); }

    REAL& x() { return m_x; }
    REAL& y() { return m_y; }
    REAL& u() { return m_x; }
    REAL& v() { return m_y; }
    REAL& operator [](int i) { return m_value[i]; }

    REAL x() const { return m_x; }
    REAL y() const { return m_y; }
    REAL u() const { return m_x; }
    REAL v() const { return m_y; }
    REAL operator [](int i) const { return m_value[i]; }

    bool operator ==(const Vector2 &v) const { return m_x == v.x() && m_y == v.y(); }

//    bool operator <(REAL v) const { return m_x < v && m_y < v && m_z < v; }
//    bool operator >(REAL v) const { return m_x > v && m_y > v && m_z > v; }

    inline Vector2 operator +(const Vector2& v) const { return Vector2(m_x + v.x(), m_y + v.y()); }
    inline Vector2 operator -(const Vector2& v) const { return Vector2(m_x - v.x(), m_y - v.y()); }
    inline Vector2 operator *(const Vector2& v) const { return Vector2(m_x * v.x(), m_y * v.y()); }
    inline REAL    operator &(const Vector2& v) const { return m_x * v.x() + m_y * v.y(); } // dot
    inline Vector2 operator /(const Vector2& v) const
    {
        return (v.x() != 0 && v.y() != 0) ? Vector2(m_x / v.x(), m_y / v.y()) : Vector2::c0;
    }

    inline Vector2 operator *(REAL val) const { return Vector2(m_x * val, m_y * val); }
    inline Vector2 operator /(REAL val) const
    {
        REAL inv = (val != 0) ? 1 / val : 0;
        return (val != 0) ? Vector2(m_x * inv, m_y * inv) : Vector2::c0;
    }

    std::string toString() const { return "[" + std::to_string(m_x) + ", " + std::to_string(m_y) + "]"; }

    friend Vector2 operator *(REAL val, const Vector2& v);

protected:
	union
	{
		struct
		{
            REAL m_x;
            REAL m_y;
		};
        REAL m_value[2];
	};
};

} //namespace Render
