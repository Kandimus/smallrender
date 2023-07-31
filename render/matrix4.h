#pragma once


#include "matrix3.h"

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
    Matrix4(const Matrix4& m) { memcpy(m_value, m.data(), sizeof(REAL) * 16); }

    Matrix4& operator =(REAL v) { m_00 = m_11 = m_22 = v; m_01 = m_02 = m_03 = m_10 = m_12 = m_13 = m_20 = m_21 = m_23 = m_30 = m_31 = m_32 = 0; m_33 = 1; return *this; }
    Matrix4& operator =(const Matrix4& m) { memcpy(&m_value, m.data(), sizeof(REAL) * 16); return *this; }
    Matrix4& operator =(const Matrix3& m)
    {
        reset();
        m_00 = m.value(0, 0); m_01 = m.value(0, 1); m_02 = m.value(0, 2);
        m_10 = m.value(1, 0); m_01 = m.value(1, 1); m_02 = m.value(1, 2);
        m_20 = m.value(2, 0); m_01 = m.value(2, 1); m_02 = m.value(2, 2);
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
    void scale(REAL x, REAL y, REAL z) { scale(Vector3(x, y, z)); }
    void scale(const Vector3& v) { *this = c1; m_00 = v.x(); m_11 = v.y(); m_22 = v.z(); }
    void rotateX(REAL);
    void rotateY(REAL);
    void rotateZ(REAL);
    void mirrorX() { *this = c1; m_00 = -1; }
    void mirrorY() { *this = c1; m_11 = -1; }
    void mirrorZ() { *this = c1; m_22 = -1; }

//	void MakeReflection(const Plane &zPlane);

    void view(const Vector3& zEye, const Vector3& zAt, const Vector3& zUp);
    void proj(REAL, REAL, REAL, REAL);

//	void ProjectToFrustum(Plane azPlane[6]);

    REAL& value(int x) { return m_value[x]; }
    REAL value(int x) const { return m_value[x]; }
    REAL& value(int x, int y) { return m_matrix[x][y]; }
    REAL  value(int x, int y) const { return m_matrix[x][y]; }

    REAL minor(int iR0, int iR1, int iR2, int iC0, int iC1, int iC2) const;
    REAL determinant() const { 	return m_00 * minor(1, 2, 3, 1, 2, 3) - m_01 * minor(1, 2, 3, 0, 2, 3) + m_02 * minor(1, 2, 3, 0, 1, 3) - m_03 * minor(1, 2, 3, 0, 1, 2); }

    Vector3 operator *(const Vector3 &v) const;

    Matrix4 adjoint() const;
    Matrix4 inverse() const { return adjoint() * (1 - determinant()); }

//	void LoadFromFile(FilePtr pFile);
//	void SaveToFile(FilePtr pFile) const;

private:
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



//ZONE_INLINE void Matrix4::MakeReflection(const Plane &zPlane)
//{
//	float fNX = zPlane.GetNormal().X();
//	float fNY = zPlane.GetNormal().Y();
//	float fNZ = zPlane.GetNormal().Z();
//	float fC = zPlane.GetConstant();

//	f_f00 = -2.0f*fNX*fNX + 1.0f;
//	f_f01 = -2.0f*fNX*fNY;
//	f_f02 = -2.0f*fNX*fNZ;
//	f_f03 = -2.0f*fNX*fC;

//	f_f10 = -2.0f*fNY*fNX;
//	f_f11 = -2.0f*fNY*fNY + 1.0f;
//	f_f12 = -2.0f*fNY*fNZ;
//	f_f13 = -2.0f*fNY*fC;

//	f_f20 = -2.0f*fNZ*fNX;
//	f_f21 = -2.0f*fNZ*fNY;
//	f_f22 = -2.0f*fNZ*fNZ + 1.0f;
//	f_f23 = -2.0f*fNZ*fC;

//	f_f30 = f_f31 = f_f32 = 0.0f;
//	f_f33 = 1.0f;
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

