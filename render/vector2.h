#pragma once

#include "defines.h"

namespace Render
{

class Vector2
{
public:
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
    inline void clip() { for(int i = 0; i < 2; ++i) if(m_value[i] <= 0) m_value[i] = 0; else if(m_value[i] >= 1) m_value[i] = 1; }

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

    friend Vector2 operator +(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator -(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator *(REAL val, const Vector2& v);
    friend Vector2 operator *(const Vector2& v, REAL val);
    friend Vector2 operator *(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator /(const Vector2& v, REAL val);
    friend Vector2 operator /(const Vector2& v1, const Vector2& v2);
    friend REAL operator &(const Vector2& v1, const Vector2& v2); // dot
//    friend Vector2 operator ^(const Vector2& v1, const Vector2& v2); // cross

    //    void load(FilePtr pFile);
    //    void save(FilePtr pFile) const;

    static Vector2 c0() { return Vector2(0, 0); };
    static Vector2 c1() { return Vector2(1, 1); };
    static Vector2 cX() { return Vector2(1, 0); };
    static Vector2 cY() { return Vector2(0, 1); };

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

inline Vector2 operator +(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x() + v2.x(), v1.y() + v2.y());
}

inline Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x() - v2.x(), v1.y() - v2.y());
}

inline Vector2 operator *(REAL val, const Vector2& v)
{
    return Vector2(val * v.x(), val * v.y());
}

inline Vector2 operator *(const Vector2& v, REAL val)
{
    return Vector2(v.x() * val, v.y() * val);
}

inline Vector2 operator *(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x() * v2.x(), v1.y() * v2.y());
}

inline Vector2 operator /(const Vector2& v, REAL val)
{
    REAL inv = (val != 0) ? 1 / val : 0;
    return (val != 0) ? Vector2(v.x() * inv, v.y() * inv) : Vector2::c0();
}

inline Vector2 operator /(const Vector2& v1, const Vector2& v2)
{
    return (v2.x() !=0 && v2.y() != 0)
               ? Vector2(v1.x() / v2.x(), v1.y() / v2.y())
               : Vector2::c0();
}

inline REAL operator &(const Vector2& v1, const Vector2& v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

}
