
#include <algorithm>

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

//NOTE not tested
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
    Matrix4 r = Matrix4::c0;

    REAL t2X = 2 * m_x;
    REAL t2Y = 2 * m_y;
    REAL t2Z = 2 * m_z;

    REAL t2WX = t2X * m_w;
    REAL t2WY = t2Y * m_w;
    REAL t2WZ = t2Z * m_w;
    REAL t2XX = t2X * m_x;
    REAL t2XY = t2Y * m_x;
    REAL t2XZ = t2Z * m_x;
    REAL t2YY = t2Y * m_y;
    REAL t2YZ = t2Z * m_y;
    REAL t2ZZ = t2Z * m_z;

    r.value(0, 0) = 1 - t2YY - t2ZZ;
    r.value(1, 0) = t2XY - t2WZ;
    r.value(2, 0) = t2XZ + t2WY;
    r.value(0, 1) = t2XY + t2WZ;
    r.value(1, 1) = 1 - t2XX - t2ZZ;
    r.value(2, 1) = t2XZ - t2WX;
    r.value(0, 2) = t2XZ - t2WY;
    r.value(1, 2) = t2YZ + t2WX;
    r.value(2, 2) = 1 - t2XX - t2YY;

    r.value(3, 3) = 1;

    return r;
}

//NOTE not tested
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

//NOTE not tested
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
// roll (x), pitch (Y), yaw (z)
void Quaternion::fromYPR(REAL yaw, REAL pitch, REAL roll)
{
//    Quaternion zYaw(0, SIN(yaw / 2), 0, COS(yaw / 2));
//    Quaternion zPitch(SIN(pitch / 2), 0, 0, COS(pitch / 2));
//    Quaternion zRoll(0, 0, SIN(roll / 2), COS(roll / 2));

//    *this = zYaw * zPitch * zRoll;
    REAL cr = COS(roll * REAL(0.5));
    REAL sr = SIN(roll * REAL(0.5));
    REAL cp = COS(pitch * REAL(0.5));
    REAL sp = SIN(pitch * REAL(0.5));
    REAL cy = COS(yaw * REAL(0.5));
    REAL sy = SIN(yaw * REAL(0.5));

    m_w = cr * cp * cy + sr * sp * sy;
    m_x = sr * cp * cy - cr * sp * sy;
    m_y = cr * sp * cy + sr * cp * sy;
    m_z = cr * cp * sy - sr * sp * cy;
}


// roll (x), pitch (Y), yaw (z)
Vector3 Quaternion::toYPR() const
{
    Vector3 a;

    // Ver 1. GLM code

    REAL y = 2.0 * (m_y * m_z + m_w * m_x);
    REAL x = m_w * m_w - m_x * m_x - m_y * m_y + m_z * m_z;
    a.x() = (x == 0 && y == 0) ? 2 * ATAN2(m_x, m_w) : ATAN2(y, x); // PITCH - X

    a.y() = ASIN(std::clamp(-2 * (m_x * m_z - m_w * m_y), -1.0, 1.0)); // YAW - Y

    y = 2.0 * (m_x * m_y + m_w * m_z);
    x = m_w * m_w + m_x * m_x - m_y * m_y - m_z * m_z;
    a.z() = (x == 0 && y == 0) ? 0 : ATAN2(y, x); // ROLL - Z

    // Ver 2. from some site
    /*
    // roll (x-axis rotation)
    REAL sinr_cosp = 2 * (m_w * m_x + m_y * m_z);
    REAL cosr_cosp = 1 - 2 * (m_x * m_x + m_y * m_y);
    a.x() = ATAN2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    REAL sinp = SQRT(1 + 2 * (m_w * m_y - m_x * m_z));
    REAL cosp = SQRT(1 - 2 * (m_w * m_y - m_x * m_z));
    a.y() = 2 * ATAN2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    REAL siny_cosp = 2 * (m_w * m_z + m_x * m_y);
    REAL cosy_cosp = 1 - 2 * (m_y * m_y + m_z * m_z);
    a.z() = ATAN2(siny_cosp, cosy_cosp);
    */

    return a;
}

//namespace Render
}
