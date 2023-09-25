#pragma once

#include "matrix3.h"
#include "plane.h"

namespace Render
{

class Matrix4
{
public:
    static const Matrix4 c0;
    static const Matrix4 c1;

    Matrix4() { reset(); }
    Matrix4(REAL v) { *this = v; }
    Matrix4(REAL f00, REAL f01, REAL f02, REAL f03, REAL f10, REAL f11, REAL f12, REAL f13,
            REAL f20, REAL f21, REAL f22, REAL f23,REAL f30, REAL f31, REAL f32, REAL f33)
        { m_00 = f00; m_01 = f01; m_02 = f02; m_03 = f03; m_10 = f10; m_11 = f11; m_12 = f12; m_13 = f13;
          m_20 = f20; m_21 = f21; m_22 = f22; m_23 = f23; m_30 = f30; m_31 = f31; m_32 = f32; m_33 = f33; }
    Matrix4(const Matrix4& m) { *this = m; }
    Matrix4(const Matrix3& m) { *this = m; }

    Matrix4& operator =(REAL v) { m_00 = m_11 = m_22 = v; m_01 = m_02 = m_03 = m_10 = m_12 = m_13 = m_20 = m_21 = m_23 = m_30 = m_31 = m_32 = 0; m_33 = 1; return *this; }
    Matrix4& operator =(const Matrix4& m) { memcpy(m_value, m.data(), sizeof(REAL) * 16); return *this; }
    Matrix4& operator =(const Matrix3& m)
    {
        reset();
        m_00 = m.value(0, 0); m_01 = m.value(0, 1); m_02 = m.value(0, 2);
        m_10 = m.value(1, 0); m_11 = m.value(1, 1); m_12 = m.value(1, 2);
        m_20 = m.value(2, 0); m_21 = m.value(2, 1); m_22 = m.value(2, 2);
        m_33 = 1.0;
        return *this;
    }

    Matrix4& operator +=(const Matrix4& m) { for(int i = 0; i < 16; ++i) m_value[i] += m.value(i); return *this; }
    Matrix4& operator -=(const Matrix4& m) { for(int i = 0; i < 16; ++i) m_value[i] -= m.value(i); return *this; }
    Matrix4& operator *=(REAL v) { for(int i = 0; i < 16; ++i) m_value[i] *= v; return *this; }
    Matrix4& operator *=(const Matrix4&);
    Matrix4& operator /=(REAL);

    Matrix4 operator *(const Matrix4& m) const { Matrix4 r(*this); r *= m; return r; }
    Matrix4 operator *(REAL v) const { Matrix4 r(*this); r *= v; return r; }

    bool operator ==(const Matrix4& m) const { for(int i = 0; i < 16; ++i) if (m_value[i] != m.value(i)) return false; return true; }

    void reset() { memset(&m_value, 0, sizeof(REAL) * 16); }
    const REAL* data() const { return m_value; }

    void translate(REAL x, REAL y, REAL z) { translate(Vector3(x, y, z)); }
    void translate(const Vector3& v) { *this = c1; m_30 = v.x(); m_31 = v.y(); m_32 = v.z(); }
    Matrix4 clearTranslate() const { Matrix4 m4 = *this; m4.m_30 = m4.m_31 = m4.m_32 = 0.0; return m4; }
    void scale(REAL x, REAL y, REAL z) { scale(Vector3(x, y, z)); }
    void scale(const Vector3& v) { *this = c1; m_00 = v.x(); m_11 = v.y(); m_22 = v.z(); }
    void rotateX(REAL);
    void rotateY(REAL);
    void rotateZ(REAL);
    void mirrorX() { *this = c1; m_00 = -1; }
    void mirrorY() { *this = c1; m_11 = -1; }
    void mirrorZ() { *this = c1; m_22 = -1; }

    void makeReflection(const Plane& p);

    void view(const Vector3& zEye, const Vector3& zAt, const Vector3& zUp);
    void proj(REAL, REAL, REAL, REAL);

//	void ProjectToFrustum(Plane azPlane[6]);

    inline REAL& value(int x) { return m_value[x]; }
    inline REAL value(int x) const { return m_value[x]; }
    inline REAL& value(int y, int x) { return m_matrix[y][x]; }
    inline REAL  value(int y, int x) const { return m_matrix[y][x]; }

    REAL minor(int iR0, int iR1, int iR2, int iC0, int iC1, int iC2) const;
    REAL determinant() const { 	return m_00 * minor(1, 2, 3, 1, 2, 3) - m_01 * minor(1, 2, 3, 0, 2, 3) + m_02 * minor(1, 2, 3, 0, 1, 3) - m_03 * minor(1, 2, 3, 0, 1, 2); } //NOTE возможно считается криво

    //friend Vector3 operator *(const Vector3 &v, const Matrix4 &m);
    Vector3 operator *(const Vector3 &v) const
    {
        if (m_33 == 1 && m_03 == 0 && m_13 == 0 && m_23 == 0)
        {
            return Vector3(v.x() * m_00 + v.y() * m_10 + v.z() * m_20 + m_30,
                           v.x() * m_01 + v.y() * m_11 + v.z() * m_21 + m_31,
                           v.x() * m_02 + v.y() * m_12 + v.z() * m_22 + m_32);
        }

        REAL invW = 1 / (v.x() * m_03 + v.y() * m_13 + v.z() * m_23 + m_33);

        return Vector3((v.x() * m_00 + v.y() * m_10 + v.z() * m_20 + m_30) * invW,
                       (v.x() * m_01 + v.y() * m_11 + v.z() * m_21 + m_31) * invW,
                       (v.x() * m_02 + v.y() * m_12 + v.z() * m_22 + m_32) * invW);
    }

    Matrix4 adjoint() const;
    Matrix4 inverse() const { return adjoint() * (1 - determinant()); }

protected:
	union
	{
		struct
		{
            REAL m_00, m_01, m_02, m_03;
            REAL m_10, m_11, m_12, m_13;
            REAL m_20, m_21, m_22, m_23;
            REAL m_30, m_31, m_32, m_33;
		};
        REAL m_matrix[4][4];
        REAL m_value[16];
	};
};


//inline Vector3 operator *(const Vector3& v, const Matrix4& m)
//{
//    if (m.m_33 == 1 && m.m_03 == 0 && m.m_13 == 0 && m.m_23 == 0)
//    {
//        return Vector3(
//            m.m_00 * v.x() + m.m_01 * v.y() + m.m_02 * v.z() + m.m_03,
//            m.m_10 * v.x() + m.m_11 * v.y() + m.m_12 * v.z() + m.m_13,
//            m.m_20 * v.x() + m.m_21 * v.y() + m.m_22 * v.z() + m.m_23);
//    }

//    REAL fInvW = 1 / (m.m_30 * v.x() + m.m_31 * v.y() + m.m_32 * v.z() + m.m_33);

//    return Vector3(
//        (m.m_00 * v.x() + m.m_01 * v.y() + m.m_02 * v.z() + m.m_03) * fInvW,
//        (m.m_10 * v.x() + m.m_11 * v.y() + m.m_12 * v.z() + m.m_13) * fInvW,
//        (m.m_20 * v.x() + m.m_21 * v.y() + m.m_22 * v.z() + m.m_23) * fInvW);
//}


//ZONE_INLINE void Matrix4::ProjectToFrustum(Plane azPlane[6])
//{
//	azPlane[1].SetNormal(Vector(
//		f_afValue[3] - f_afValue[0],
//		f_afValue[7] - f_afValue[4],
//		f_afValue[11] - f_afValue[8]));
//	azPlane[1].SetConstant(f_afValue[15] - f_afValue[12]);
//	azPlane[1].Normalize();

//	azPlane[0].SetNormal(Vector(
//		f_afValue[3] + f_afValue[0],
//		f_afValue[7] + f_afValue[4],
//		f_afValue[11] + f_afValue[8]));
//	azPlane[0].SetConstant(f_afValue[15] + f_afValue[12]);
//	azPlane[0].Normalize();

//	azPlane[2].SetNormal(Vector(
//		f_afValue[3] + f_afValue[1],
//		f_afValue[7] + f_afValue[5],
//		f_afValue[11] + f_afValue[9]));
//	azPlane[2].SetConstant(f_afValue[15] + f_afValue[13]);
//	azPlane[2].Normalize();

//	azPlane[3].SetNormal(Vector(
//		f_afValue[3] - f_afValue[1],
//		f_afValue[7] - f_afValue[5],
//		f_afValue[11] - f_afValue[9]));
//	azPlane[3].SetConstant(f_afValue[15] - f_afValue[13]);
//	azPlane[3].Normalize();

//	azPlane[4].SetNormal(Vector(
//		f_afValue[3] - f_afValue[2],
//		f_afValue[7] - f_afValue[6],
//		f_afValue[11] - f_afValue[10]));
//	azPlane[4].SetConstant(f_afValue[15] - f_afValue[14]);
//	azPlane[4].Normalize();

//	azPlane[5].SetNormal(Vector(
//		f_afValue[3] + f_afValue[2],
//		f_afValue[7] + f_afValue[6],
//		f_afValue[11] + f_afValue[10]));
//	azPlane[5].SetConstant(f_afValue[15] + f_afValue[14]);
//	azPlane[5].Normalize();
//}


//namespace Render
}

