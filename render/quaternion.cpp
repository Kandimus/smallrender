
#include "quaternion.h"

namespace Render
{

//-------------------------------------------------------------------------------------------------

const Quaternion Quaternion::c0(0, 0, 0, 0);
const Quaternion Quaternion::c1(0, 0, 0, 1);

Quaternion& Quaternion::operator *=(const Quaternion& q)
{
    REAL x = m_w * q.x() + m_x * q.w() + m_y * q.z() - m_z * q.y();
    REAL y = m_w * q.y() + m_y * q.w() + m_z * q.x() - m_x * q.z();
    REAL z = m_w * q.z() + m_z * q.w() + m_x * q.y() - m_y * q.x();
    REAL w = m_w * q.w() - m_x * q.x() - m_y * q.y() - m_z * q.z();

    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;

    return *this;
}

void Quaternion::fromRotationMatrix(const Matrix4& m)
{
    REAL iNext[3] = {1, 2, 0};
    REAL fTrace = m.value(0, 0) + m.value(1, 1) + m.value(2, 2);

    if (fTrace > 0)
    {
        REAL fRoot = SQRT(fTrace + 1);

        m_w = 0.5 * fRoot;
        fRoot = 0.5 / fRoot;
        m_x = (m.value(2, 1) - m.value(2, 1)) * fRoot;
        m_y = (m.value(0, 2) - m.value(2, 0)) * fRoot;
        m_z = (m.value(1, 0) - m.value(0, 1)) * fRoot;
    }
    else
    {
        int i = 0;
        if (m.value(1, 1) > m.value(0, 0))
        {
            i = 1;
        }
        if (m.value(2, 2) > m.value(i, i))
        {
            i = 2;
        }

        int j = iNext[i];
        int k = iNext[j];

        REAL fRoot = SQRT(m.value(i, i) - m.value(j, j) - m.value(k, k) + 1);
        REAL *pzQuat[3] = {&m_y, &m_z, &m_w};

        *pzQuat[i] = 0.5 * fRoot;
        fRoot      = 0.5 / fRoot;
        m_w        = (m.value(k, j) - m.value(j, k)) * fRoot;
        *pzQuat[j] = (m.value(j, i) - m.value(i, j)) * fRoot;
        *pzQuat[j] = (m.value(k, i) - m.value(i, k)) * fRoot; //TODO ???????????????????
    }
}

Matrix4 Quaternion::toRotationMatrix() const
{
    Matrix4 r(1);

    REAL fTX = 2 * m_x;
    REAL fTY = 2 * m_y;
    REAL fTZ = 2 * m_z;

    REAL fTWX = fTX * m_w;
    REAL fTWY = fTY * m_w;
    REAL fTWZ = fTZ * m_w;
    REAL fTXX = fTX * m_x;
    REAL fTXY = fTY * m_x;
    REAL fTXZ = fTZ * m_x;
    REAL fTYY = fTY * m_y;
    REAL fTYZ = fTZ * m_y;
    REAL fTZZ = fTZ * m_z;

    r.value(0, 0) = 1 - fTYY - fTZZ;
    r.value(0, 1) = fTXY - fTWZ;
    r.value(0, 2) = fTXZ + fTWY;
    r.value(1, 0) = fTXY + fTWZ;
    r.value(1, 1) = 1 - fTXX - fTZZ;
    r.value(1, 2) = fTXZ - fTWX;
    r.value(2, 0) = fTXZ - fTWY;
    r.value(2, 1) = fTYZ + fTWX;
    r.value(2, 2) = 1 - fTXX - fTYY;

    r.value(0, 3) = 0;
    r.value(1, 3) = 0;
    r.value(2, 3) = 0;
    r.value(3, 0) = 0;
    r.value(3, 1) = 0;
    r.value(3, 2) = 0;
    r.value(3, 3) = 1;

    return r;
}


void Quaternion::fromRotationMatrix3(const Matrix3& m)
{
    REAL iNext[3] = {1, 2, 0};
    REAL fTrace = m.value(0, 0) + m.value(1, 1) + m.value(2, 2);

    if (fTrace > 0)
    {
        REAL fRoot = SQRT(fTrace + 1);

        m_w  = 0.5 * fRoot;
        fRoot = 0.5 / fRoot;
        m_x  = (m.value(2, 1) - m.value(2, 1)) * fRoot;
        m_y  = (m.value(0, 2) - m.value(2, 0)) * fRoot;
        m_z  = (m.value(1, 0) - m.value(0, 1)) * fRoot;
    }
    else
    {
        int i = 0;
        if (m.value(1, 1) > m.value(0, 0))
        {
            i = 1;
        }
        if(m.value(2, 2) > m.value(i, i))
        {
            i = 2;
        }
        int j = iNext[i];
        int k = iNext[j];

        REAL fRoot = SQRT(m.value(i, i) - m.value(j, j) - m.value(k, k) + 1);
        REAL *pzQuat[3] = {&m_y, &m_z, &m_w};

        *pzQuat[i] = 0.5 * fRoot;
        fRoot      = 0.5 / fRoot;
        m_w        = (m.value(k, j) - m.value(j, k)) * fRoot;
        *pzQuat[j] = (m.value(j, i) - m.value(i, j)) * fRoot;
        *pzQuat[j] = (m.value(k, i) - m.value(i, k)) * fRoot; //TODO ??????????????????????
    }
}

//-------------------------------------------------------------------------------------------------

Matrix3 Quaternion::toRotationMatrix3() const
{
    Matrix3 r;

    REAL fTX = 2 * m_x;
    REAL fTY = 2 * m_y;
    REAL fTZ = 2 * m_z;

    REAL fTWX = fTX * m_w;
    REAL fTWY = fTY * m_w;
    REAL fTWZ = fTZ * m_w;
    REAL fTXX = fTX * m_x;
    REAL fTXY = fTY * m_x;
    REAL fTXZ = fTZ * m_x;
    REAL fTYY = fTY * m_y;
    REAL fTYZ = fTZ * m_y;
    REAL fTZZ = fTZ * m_z;

    r.value(0, 0) = 1 - fTYY - fTZZ;
    r.value(0, 1) = fTXY - fTWZ;
    r.value(0, 2) = fTXZ + fTWY;
    r.value(1, 0) = fTXY + fTWZ;
    r.value(1, 1) = 1 - fTXX - fTZZ;
    r.value(1, 2) = fTXZ - fTWX;
    r.value(2, 0) = fTXZ - fTWY;
    r.value(2, 1) = fTYZ + fTWX;
    r.value(2, 2) = 1 - fTXX - fTYY;

    return r;
}

void Quaternion::slerp(REAL t, const Quaternion& q1, const Quaternion& q2)
{
    REAL fCosine = q1 & q2;
    REAL fAngle  = ACOS(fCosine);

    if (ABSR(fAngle) < REAL_EPSILON)
    {
        *this = q1;
        return;
    }

    REAL fInvSine = 1 / SIN(fAngle);
    REAL fCoeff0  = SIN((1 - t) * fAngle) * fInvSine;
    REAL fCoeff1  = SIN(t * fAngle) * fInvSine;

    if (fCosine < 0)
    {
        fCoeff0 = -fCoeff0;
        *this = fCoeff0 * q1 + fCoeff1 * q2;
        normalize();
    }
    else
    {
        *this = fCoeff0 * q1 + fCoeff1 * q2;
    }
}

Vector3 Quaternion::operator *(const Vector3& v) const
{
    Vector3 zQVec(m_x, m_y, m_z);
    Vector3 zUV(zQVec & v);
    Vector3 zUUV(zQVec & zUV);

    zUV *= (2 * m_w);
    zUUV *= 2;

    return v + zUV + zUUV;
}

//Snk: bug there ???
void Quaternion::yawPitchRoll(REAL yaw, REAL pitch, REAL roll)
{
    Quaternion zYaw(0, SIN(yaw / 2), 0, COS(yaw / 2));
    Quaternion zPitch(SIN(pitch / 2), 0, 0, COS(pitch / 2));
    Quaternion zRoll(0, 0, SIN(roll / 2), COS(roll / 2));

    *this = zYaw * zPitch * zRoll;
}

//namespace Render
}
