#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "util.h"


namespace Render
{

class Vector3
{
friend Vector3 operator *(REAL val, const Vector3& v);

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
    inline Vector3& operator *=(const Vector3& v) { m_x *= v.x(); m_y *= v.y(); m_z *= v.z(); return *this; }
    inline Vector3& operator /=(const Vector3& v) { m_x = v.x() != 0 ? m_x / v.x() : 0; m_y = v.y() != 0 ? m_y / v.y() : 0; m_z = v.z() != 0 ? m_z / v.z() : 0; return *this; }
    inline Vector3& operator *=(REAL v) { m_x *= v; m_y *= v; m_z *= v; return *this; }
    inline Vector3& operator /=(REAL v) { if (v == 0) { m_x = 0; m_y = 0; m_z = 0; } else {REAL iv = 1 / v; m_x *= iv; m_y *= iv; m_z *= iv; } return *this; }

    inline Vector3 operator -() const { return Vector3(-m_x, -m_y, -m_z); }

    inline Vector3 reflect(const Vector3 &v) const { return (*this) - REAL(2.0) * ((*this) & v) * v; } //NOTE отражение?

    const REAL* data() const { return m_value; }
    REAL length() const {return SQRT(squaredLength()); } //NOTE: look to std::hypot
    inline REAL squaredLength() const { return m_x * m_x + m_y * m_y + m_z * m_z; }
    inline void normalize() { REAL l = length(); if(l != 0) (*this) *= 1 / l; else (*this) = 0; }
    inline void clamp() { for(int i = 0; i < 3; ++i) m_value[i] = Util::clamp(m_value[i]); }
    inline Vector3& clampMax(REAL v) { for(int i = 0; i < 3; ++i) m_value[i] = std::max(v, m_value[i]); return *this; }
    inline Vector3& clampMin(REAL v) { for(int i = 0; i < 3; ++i) m_value[i] = std::min(v, m_value[i]); return *this; }

    inline Vector3 min(const Vector3& v) { return Vector3(std::min(m_x, v.x()), std::min(m_y, v.y()), std::min(m_z, v.z())); }
    inline Vector3 max(const Vector3& v) { return Vector3(std::max(m_x, v.x()), std::max(m_y, v.y()), std::max(m_z, v.z())); }


    std::string toString() const { return "[" + std::to_string(m_x) + ", " + std::to_string(m_y) + ", " + std::to_string(m_z) + "]"; }

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

    Vector3 operator +(const Vector3& v) const { return Vector3(m_x + v.x(), m_y + v.y(), m_z + v.z()); }
    Vector3 operator -(const Vector3& v) const { return Vector3(m_x - v.x(), m_y - v.y(), m_z - v.z()); }
    Vector3 operator *(const Vector3& v) const { return Vector3(m_x * v.x(), m_y * v.y(), m_z * v.z()); }
    Vector3 operator /(const Vector3& v) const { return (v.x() !=0 && v.y() != 0 && v.z() != 0) ? Vector3(m_x / v.x(), m_y / v.y(), m_z / v.z()) : Vector3::c0; }
    REAL    operator &(const Vector3& v) const { return m_x * v.x() + m_y * v.y() + m_z * v.z(); } // dot
    Vector3 operator ^(const Vector3& v) const { return Vector3(m_y * v.z() - m_z * v.y(), m_z * v.x() - m_x * v.z(), m_x * v.y() - m_y * v.x()); } // cross

    Vector3 operator *(REAL val) const { return Vector3(m_x * val, m_y * val, m_z * val); }
    Vector3 operator /(REAL val) const { REAL inv = (val != 0) ? 1 / val : 0; return Vector3(m_x * inv, m_y * inv, m_z * inv); }

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

inline Vector3 operator +(REAL val, const Vector3& v)
{
    return Vector3(val + v.x(), val + v.y(), val + v.z());
}

inline Vector3 operator -(REAL val, const Vector3& v)
{
    return Vector3(val - v.x(), val - v.y(), val - v.z());
}

inline Vector3 operator *(REAL val, const Vector3& v)
{
    return Vector3(val * v.x(), val * v.y(), val * v.z());
}

inline std::vector<Vector3>& operator +=(std::vector<Vector3>& vec, const Vector3& val)
{
    for (auto& item : vec)
    {
        item += val;
    }
    return vec;
}

}
