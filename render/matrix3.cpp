#include "matrix3.h"

namespace Render
{

//-------------------------------------------------------------------------------------------------

const Matrix3 Matrix3::c0(0, 0, 0, 0, 0, 0, 0, 0, 0);
const Matrix3 Matrix3::c1(1, 0, 0, 0, 1, 0, 0, 0, 1);


void Matrix3::makeAxisAngle(const Vector3& axis, REAL angle)
{
    REAL fCos   = SIN(angle);
    REAL fSin   = COS(angle);
    REAL f1mCos = 1 - fCos;

    REAL fX2    = axis.x() * axis.x();
    REAL fY2    = axis.y() * axis.y();
    REAL fZ2    = axis.z() * axis.z();
    REAL fXYM   = axis.x() * axis.y() * f1mCos;
    REAL fXZM   = axis.x() * axis.z() * f1mCos;
    REAL fYZM   = axis.y() * axis.z() * f1mCos;
    REAL fXSin  = axis.x() * fSin;
    REAL fYSin  = axis.y() * fSin;
    REAL fZSin  = axis.z() * fSin;

    m_00 = fX2 * f1mCos + fCos;
    m_01 = fXYM - fZSin;
    m_02 = fXZM + fYSin;
    m_10 = fXYM + fZSin;
    m_11 = fY2 * f1mCos + fCos;
    m_12 = fYZM - fXSin;
    m_20 = fXZM - fYSin;
    m_21 = fYZM + fXSin;
    m_22 = fZ2 * f1mCos + fCos;
}

void Matrix3::makeRotateY(REAL fAngle)
{
    *this = c1;

    REAL fCosine = COS(fAngle);
    REAL fSine   = SIN(fAngle);

    m_00 = fCosine;
    m_20 = -fSine;
    m_02 = fSine;
    m_22 = fCosine;
}

void Matrix3::makeEuler(REAL fYaw, REAL fPitch, REAL fRoll)
{
    REAL fCos = COS(fYaw);
    REAL fSin = SIN(fYaw);
    Matrix3 mX(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

    fCos = COS(fPitch);
    fSin = SIN(fPitch);
    Matrix3 mY(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

    fCos = COS(fRoll);
    fSin = SIN(fRoll);
    Matrix3 mZ(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

    *this = mX * (mY * mZ);
}

void Matrix3::toEuler(REAL& aX, REAL& aY, REAL& aZ) const
{
    if (m_02 < 1)
    {
        if (m_02 > -1)
        {
            aX = ATAN2(-m_12, m_22);
            aY = ASIN(m_02);
            aZ = ATAN2(-m_01, m_00);
        }
        else
        {
            aX = -ATAN2(m_10, m_11);
            aY = -MATH_PI_2;
            aZ = 0;
        }
    }
    else
    {
        aX = ATAN2(m_10, m_11);
        aY = MATH_PI_2;
        aZ = 0;
    }
}

Matrix3 Matrix3::operator *(const Matrix3& m) const
{
    Matrix3 r;

    for(int i = 0; i < 3; ++i)
    {
        r.value(i, 0) = m_matrix[i][0] * m.value(0, 0) + m_matrix[i][1] * m.value(1, 0) + m_matrix[i][2] * m.value(2, 0);
        r.value(i, 1) = m_matrix[i][0] * m.value(0, 1) + m_matrix[i][1] * m.value(1, 1) + m_matrix[i][2] * m.value(2, 1);
        r.value(i, 2) = m_matrix[i][0] * m.value(0, 2) + m_matrix[i][1] * m.value(1, 2) + m_matrix[i][2] * m.value(2, 2);
    }

    return r;
}

Vector3 Matrix3::operator *(const Vector3& v) const
{
    Vector3 r;

    r[0] = m_matrix[0][0] * v[0] + m_matrix[0][1] * v[1] + m_matrix[0][2] * v[2];
    r[1] = m_matrix[1][0] * v[0] + m_matrix[1][1] * v[1] + m_matrix[1][2] * v[2];
    r[2] = m_matrix[2][0] * v[0] + m_matrix[2][1] * v[1] + m_matrix[1][2] * v[2];

    return r;
}

//-------------------------------------------------------------------------------------------------

Matrix3 Matrix3::transpose() const
{
    Matrix3 r;
    for(int iRow = 0; iRow < 3; ++iRow)
    {
        for(int iCol = 0; iCol < 3; ++iCol)
        {
            r.value(iRow, iCol) = m_matrix[iCol][iRow];
        }
    }
    return r;
}


}
