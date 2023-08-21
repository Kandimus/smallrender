//=================================================================================================
//===
//=== Zone_Matrix4.cpp
//===
//=== Copyright (c) 2003-2005 by RangeSoft.
//=== All rights reserved.
//===
//=== Majdanov "Snake" Dmitrij V.
//===
//=================================================================================================

#include "matrix4.h"

namespace Render
{

//-------------------------------------------------------------------------------------------------

const Matrix4 Matrix4::c0(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const Matrix4 Matrix4::c1(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);


Matrix4& Matrix4::operator *=(const Matrix4& m)
{
    Matrix4 tmp = *this;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            REAL Sum = 0;

            for (int k = 0; k < 4; ++k)
            {
                Sum += tmp.value(i, k) * m.value(k, j);
            }
            m_matrix[i][j] = Sum;
        }
    }

    return *this;
}

Matrix4& Matrix4::operator /=(REAL v)
{
    if(v == 0)
    {
        *this = Matrix4::c0;
    }

    REAL invValue = 1 / v;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            m_matrix[i][j] *= invValue;
        }
    }

    return *this;
}


void Matrix4::rotateX(REAL a)
{
    *this = c1;

    REAL fCosine = COS(a);
    REAL fSine   = SIN(a);

    m_11 = fCosine;
    m_21 = -fSine;
    m_12 = fSine;
    m_22 = fCosine;
}

void Matrix4::rotateY(REAL a)
{
    *this = c1;

    REAL fCosine = COS(a);
    REAL fSine   = SIN(a);

    m_00 = fCosine;
    m_20 = -fSine;
    m_02 = fSine;
    m_22 = fCosine;
}

void Matrix4::rotateZ(REAL a)
{
    *this = c1;

    REAL fCosine = COS(a);
    REAL fSine   = SIN(a);

    m_00 = fCosine;
    m_10 = -fSine;
    m_01 = fSine;
    m_11 = fCosine;
}

void Matrix4::makeReflection(const Plane& p)
{
    REAL fNX = p.normal().x();
    REAL fNY = p.normal().y();
    REAL fNZ = p.normal().z();
    REAL fC  = p.constant();

    m_00 = -2 * fNX * fNX + 1;
    m_01 = -2 * fNX * fNY;
    m_02 = -2 * fNX * fNZ;
    m_03 = -2 * fNX * fC;

    m_10 = -2 * fNY * fNX;
    m_11 = -2 * fNY * fNY + 1;
    m_12 = -2 * fNY * fNZ;
    m_13 = -2 * fNY * fC;

    m_20 = -2 * fNZ * fNX;
    m_21 = -2 * fNZ * fNY;
    m_22 = -2 * fNZ * fNZ + 1;
    m_23 = -2 * fNZ * fC;

    m_30 = m_31 = m_32 = 0;
    m_33 = 1;
}


void Matrix4::view(const Vector3& zEye, const Vector3& zAt, const Vector3& zUp)
{
    Vector3 zZAxis = zAt - zEye;
    zZAxis.normalize();

    Vector3 zXAxis = zUp ^ zZAxis;
    zXAxis.normalize();

    Vector3 zYAxis = zZAxis ^ zXAxis;

    m_00 = zXAxis.x();
    m_10 = zXAxis.y();
    m_20 = zXAxis.z();
    m_30 = -zXAxis & zEye;

    m_01 = zYAxis.x();
    m_11 = zYAxis.y();
    m_21 = zYAxis.z();
    m_31 = -zYAxis & zEye;

    m_02 = zZAxis.x();
    m_12 = zZAxis.y();
    m_22 = zZAxis.z();
    m_32 = -zZAxis & zEye;

    m_03 = m_13 = m_23 = 0;
    m_33 = 1;
}

void Matrix4::proj(REAL fNear, REAL fFar, REAL fHorFOV, REAL fVerFOV)
{
    REAL fW = 1 / TAN(fHorFOV * 0.5);
    REAL fH = 1 / TAN(fVerFOV * 0.5);
    REAL fQ = fFar/(fFar - fNear);

    m_00 = fW;
    m_11 = fH;
    m_22 = fQ;
    m_32 = -fQ * fNear;
    m_23 = 1;

    m_01 = m_02 = m_03 = m_10 = m_12 = m_13 = m_20 = m_21 = m_30 = m_31 = m_33 = 0;
}

REAL Matrix4::minor(int iR0, int iR1, int iR2, int iC0, int iC1, int iC2) const
{
    const REAL &f00 = m_matrix[iR0][iC0];
    const REAL &f01 = m_matrix[iR0][iC1];
    const REAL &f02 = m_matrix[iR0][iC2];
    const REAL &f10 = m_matrix[iR1][iC0];
    const REAL &f11 = m_matrix[iR1][iC1];
    const REAL &f12 = m_matrix[iR1][iC2];
    const REAL &f20 = m_matrix[iR2][iC0];
    const REAL &f21 = m_matrix[iR2][iC1];
    const REAL &f22 = m_matrix[iR2][iC2];

    return f00 * (f11 * f22 - f21 * f12) - f01 * (f10 * f22 - f20 * f12) + f02 * (f10 * f21 - f20 * f11);
}


Matrix4 Matrix4::adjoint() const
{
    return Matrix4(minor(1, 2, 3, 1, 2, 3),
                   -minor(0, 2, 3, 1, 2, 3),
                   minor(0, 1, 3, 1, 2, 3),
                   -minor(0, 1, 2, 1, 2, 3),

                   -minor(1, 2, 3, 0, 2, 3),
                   minor(0, 2, 3, 0, 2, 3),
                   -minor(0, 1, 3, 0, 2, 3),
                   minor(0, 1, 2, 0, 2, 3),

                   minor(1, 2, 3, 0, 1, 3),
                   -minor(0, 2, 3, 0, 1, 3),
                   minor(0, 1, 3, 0, 1, 3),
                   -minor(0, 1, 2, 0, 1, 3),

                   -minor(1, 2, 3, 0, 1, 2),
                   minor(0, 2, 3, 0, 1, 2),
                   -minor(0, 1, 3, 0, 1, 2),
                   minor(0, 1, 2, 0, 1, 2));
}


}
