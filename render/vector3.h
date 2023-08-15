#pragma once

#include <cmath>
#include <string>
#include "defines.h"

namespace Render
{

class Vector3
{
public:
    static Vector3 c0;
    static Vector3 c1;
    static Vector3 cX;
    static Vector3 cY;
    static Vector3 cZ;

    Vector3() {}
    Vector3(REAL v) { m_x = m_y = m_z = v; }
    Vector3(REAL x, REAL y, REAL z)  { m_x = x; m_y = y; m_z = z; }
    Vector3(const Vector3& v) { m_x = v.x(); m_y = v.y(); m_z = v.z(); }

    inline Vector3& operator =(REAL value) { m_x = m_y = m_z = value; return *this; }
    inline Vector3& operator =(const Vector3& v) { m_x = v.x(); m_y = v.y(); m_z = v.z(); return *this; }

    inline Vector3& operator +=(const Vector3& v) { m_x += v.x(); m_y += v.y(); m_z += v.z(); return *this; }
    inline Vector3& operator -=(const Vector3& v) { m_x -= v.x(); m_y -= v.y(); m_z -= v.z(); return *this; }
    inline Vector3& operator *=(REAL v) { m_x *= v; m_y *= v; m_z *= v; return *this; }
    inline Vector3& operator *=(const Vector3& v) { m_x *= v.x(); m_y *= v.y(); m_z *= v.z(); return *this; }
    inline Vector3& operator /=(REAL v) { if (v == 0) { m_x = 0; m_y = 0; m_z = 0; } else {REAL iv = 1 / v; m_x *= iv; m_y *= iv; m_z *= iv; } return *this; }

    inline Vector3 operator -() const { return Vector3(-m_x, -m_y, -m_z); }

    inline Vector3 reflect(const Vector3 &v) const { return (*this) - REAL(2.0) * ((*this) & v) * v; }

    const REAL* data() const { return m_value; }
    std::string toString() const { return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ", " + std::to_string(m_z) + ")"; }
    REAL length() const {return SQRT(squaredLength()); }
    inline REAL squaredLength() const { return m_x * m_x + m_y * m_y + m_z * m_z; }
    inline void normalize() { REAL l = length(); if(l != 0) (*this) *= 1 / l; else (*this) = 0; }
    inline void clip() { for(int i = 0; i < 3; ++i) if(m_value[i] <= 0) m_value[i] = 0; else if(m_value[i] >= 1) m_value[i] = 1; }

    REAL& x() { return m_x; }
    REAL& y() { return m_y; }
    REAL& z() { return m_z; }
    REAL& operator [](int i) { return m_value[i]; }

    REAL x() const { return m_x; }
    REAL y() const { return m_y; }
    REAL z() const { return m_z; }
    REAL operator [](int i) const { return m_value[i]; }

    bool operator ==(const Vector3 &v) const { return m_x == v.x() && m_y == v.y() && m_z == v.z(); }

    bool operator <(REAL v) const { return m_x < v && m_y < v && m_z < v; }
    bool operator >(REAL v) const { return m_x > v && m_y > v && m_z > v; }

    friend Vector3 operator +(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator -(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator *(REAL val, const Vector3& v);
    friend Vector3 operator *(const Vector3& v, REAL val);
    friend Vector3 operator *(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator /(const Vector3& v, REAL val);
    friend Vector3 operator /(const Vector3& v1, const Vector3& v2);
    friend REAL operator &(const Vector3& v1, const Vector3& v2); // dot
    friend Vector3 operator ^(const Vector3& v1, const Vector3& v2); // cross

    //    void load(FilePtr pFile);
    //    void save(FilePtr pFile) const;

protected:
	union
	{
		struct
		{
            REAL m_x;
            REAL m_y;
            REAL m_z;
		};
        REAL m_value[3];
	};
};

inline Vector3 operator +(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline Vector3 operator -(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline Vector3 operator *(REAL val, const Vector3& v)
{
    return Vector3(val * v.x(), val * v.y(), val * v.z());
}

inline Vector3 operator *(const Vector3& v, REAL val)
{
    return Vector3(v.x() * val, v.y() * val, v.z() * val);
}

inline Vector3 operator *(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline Vector3 operator /(const Vector3& v, REAL val)
{
    REAL inv = (val != 0) ? 1 / val : 0;
    return (val != 0) ? Vector3(v.x() * inv, v.y() * inv, v.z() * inv) : Vector3::c0;
}

inline Vector3 operator /(const Vector3& v1, const Vector3& v2)
{
    return (v2.x() !=0 && v2.y() != 0 && v2.z() != 0)
               ? Vector3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z())
               : Vector3::c0;
}

inline REAL operator &(const Vector3& v1, const Vector3& v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline Vector3 operator ^(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
        v1.y() * v2.z() - v1.z() * v2.y(),
        v1.z() * v2.x() - v1.x() * v2.z(),
        v1.x() * v2.y() - v1.y() * v2.x());
}

}
