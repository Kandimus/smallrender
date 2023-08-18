#pragma once

#include "matrix4.h"

namespace Render
{

class Quaternion
{
public:
	static const Quaternion c0;
	static const Quaternion c1;

    Quaternion() = default;
    Quaternion(REAL x, REAL y, REAL z, REAL w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
    Quaternion(const Vector3& v, REAL w) : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_w(w) { }
    Quaternion(const Matrix4& m) { fromRotationMatrix(m); }
    Quaternion(const Quaternion& q) { *this = q; }

    Quaternion& operator =(REAL v) { m_x = m_y = m_z = m_w = v; return *this; }
    Quaternion& operator =(const Quaternion& q) { memcpy(m_value, q.data(), sizeof(REAL) * 4); return *this; }

    Quaternion& operator +=(const Quaternion& q) { m_x += q.x(); m_y += q.y(); m_z += q.z(); m_w += q.w(); return *this; }
    Quaternion& operator -=(const Quaternion& q) { m_x -= q.x(); m_y -= q.y(); m_z -= q.z(); m_w -= q.w(); return *this; }
    Quaternion& operator *=(REAL v) { m_x *= v; m_y *= v; m_z *= v; m_w *= v; return *this; }
    Quaternion& operator *=(const Quaternion& q);
    Quaternion& operator /=(REAL v) { 	if(v == 0) *this = c0; REAL invValue = 1 / v; m_x *= invValue; m_y *= invValue; m_z *= invValue; m_w *= invValue; return *this; }

    const REAL* data() const { return m_value; }

    void fromRotationMatrix(const Matrix4& m);
    Matrix4 toRotationMatrix() const;

    void fromRotationMatrix3(const Matrix3& m);
    Matrix3 toRotationMatrix3() const;

    Vector3 direction() const { return Vector3(2 * (m_x * m_z - m_w * m_y), 2 * (m_y * m_z + m_w * m_x), 1 - 2 * (m_x * m_x + m_y * m_y)); }

    void slerp(REAL t, const Quaternion& q1, const Quaternion& q2);

    void normalize() { 	REAL f = 1 / SQRT(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w); *this *= f; }

    Quaternion conjugate() const { return Quaternion(m_w, -m_x, -m_y, -m_z); }
    Quaternion inverse() const { return conjugate() / (*this & *this); }

    REAL& x() {  return m_x; }
    REAL& y() {  return m_y; }
    REAL& z() {  return m_z; }
    REAL& w() {  return m_w; }
    REAL& operator [](int i)  {  return m_value[i]; }

    REAL x() const {  return m_x; }
    REAL y() const {  return m_y; }
    REAL z() const {  return m_z; }
    REAL w() const {  return m_w; }
    REAL operator [](int i) const {  return m_value[i]; }

    Quaternion operator -() const { return Quaternion(-m_x, -m_y, -m_z, -m_w); }

    Vector3 operator *(const Vector3& v) const;

    void fromYPR(REAL yaw, REAL pitch, REAL roll);
    void fromYPR(Vector3 ypr) { fromYPR(ypr.z(), ypr.y(), ypr.x()); }
    Vector3 toYPR() const;

    bool operator ==(const Quaternion& q) const { return m_x == q.x() && m_y == q.y() && m_z == q.z() && m_w == q.w(); }

    Quaternion operator +(const Quaternion& q) const { return Quaternion(m_x + q.x(), m_y + q.y(), m_z + q.z(), m_w + q.w()); }
    Quaternion operator -(const Quaternion& q) const { return Quaternion(m_x - q.x(), m_y - q.y(), m_z - q.z(), m_w - q.w()); }
    REAL       operator &(const Quaternion& q) const { return m_x * q.x() + m_y * q.y() + m_z * q.z() + m_w * q.w(); } // dot
    Quaternion operator *(const Quaternion& q) const
    {
        return Quaternion(
            m_w * q.x() + m_x * q.w() + m_y * q.z() - m_z * q.y(),
            m_w * q.y() + m_y * q.w() + m_z * q.x() - m_x * q.z(),
            m_w * q.z() + m_z * q.w() + m_x * q.y() - m_y * q.x(),
            m_w * q.w() - m_x * q.x() - m_y * q.y() - m_z * q.z());
    }
    Quaternion operator *(REAL v) const { return Quaternion(m_x * v, m_y * v, m_z * v, m_w * v); }
    Quaternion operator /(REAL v) const { if (v == 0) return Quaternion::c0; REAL iv = 1 / v; return Quaternion(m_x * iv, m_y * iv, m_z * iv, m_w * iv); }

    std::string toString() const { return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ", " + std::to_string(m_z) + "; " + std::to_string(m_w) + ")"; }

    friend Quaternion operator *(REAL, const Quaternion&);

private:
	union
	{
		struct
		{
            REAL m_x;
            REAL m_y;
            REAL m_z;
            REAL m_w;
		};
        REAL m_value[4];
	};
};

inline Quaternion operator *(REAL v, const Quaternion& q)
{
    return Quaternion(v * q.x(), v * q.y(), v * q.z(), v * q.w());
}


//namespace Render
}

