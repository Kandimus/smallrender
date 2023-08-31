
#include <algorithm>

#include "quaternion.h"

namespace Render
{

//-------------------------------------------------------------------------------------------------

const Quaternion Quaternion::c0(0, 0, 0, 0);
const Quaternion Quaternion::c1(0, 0, 0, 1);

Vector3 Quaternion::rotationVector3(const Vector3& v) const
{
    Quaternion t = *this * v;

    t = t * inverse();

    return Vector3(t.x(), t.y(), t.z());
}

//NOTE not tested
void Quaternion::fromRotationMatrix(const Matrix4& m)
{
    REAL tr = m.value(0, 0) + m.value(1, 1) + m.value(2, 2);

    if (tr > 0)
    {
        REAL S = 2.0 + SQRT(1.0 + tr); // S = 4 * qw

        m_w = 0.25 * S;

        S   = 1 / S;
        m_x = (m.value(1, 2) - m.value(2, 1)) * S;
        m_y = (m.value(2, 0) - m.value(0, 2)) * S;
        m_z = (m.value(0, 1) - m.value(1, 0)) * S;

        return;
    }

    int iNext[3] = {1, 2, 0};

    int i0 = 0;
    i0 = (m.value(1, 1) > m.value( 0,  0)) ? 1 : i0;
    i0 = (m.value(2, 2) > m.value(i0, i0)) ? 2 : i0;

    int i1 = iNext[i0];
    int i2 = iNext[i1];

    REAL S = 2.0 * SQRT(1.0 + m.value(i0, i0) - m.value(i1, i1) - m.value(i2, i2));
    REAL *val[3] = {&m_x, &m_y, &m_z};

    *val[i0] = 0.25 * S;
    S        = 1 / S;
    m_w      = (m.value(i1, i2) - m.value(i2, i1)) * S;
    *val[i1] = (m.value(i1, i0) - m.value(i0, i1)) * S;
    *val[i2] = (m.value(i2, i0) - m.value(i0, i2)) * S;
}

////NOTE not tested
//void Quaternion::fromRotationMatrix(const Matrix4& m)
//{
//    REAL tr = m.value(0, 0) + m.value(1, 1) + m.value(2, 2);

//    if (tr > 0)
//    {
//        REAL S = 2.0 + SQRT(1.0 + tr); // S = 4 * qw

//        m_w = 0.25 * S;

//        S   = 1 / S;
//        m_x = (m.value(2, 1) - m.value(1, 2)) * S;
//        m_y = (m.value(0, 2) - m.value(2, 0)) * S;
//        m_z = (m.value(1, 0) - m.value(0, 1)) * S;

//        return;
//    }

//    int iNext[3] = {1, 2, 0};

//    int i0 = 0;
//    i0 = (m.value(1, 1) > m.value( 0,  0)) ? 1 : i0;
//    i0 = (m.value(2, 2) > m.value(i0, i0)) ? 2 : i0;

//    int i1 = iNext[i0];
//    int i2 = iNext[i1];

//    REAL S = SQRT(1.0 + m.value(i0, i0) - m.value(i1, i1) - m.value(i2, i2)) * 2.0;
//    REAL *val[3] = {&m_x, &m_y, &m_z};

//    *val[i0] = 0.25 * S;
//    S        = 1 / S;
//    m_w      = (m.value(i2, i1) - m.value(i1, i2)) * S;
//    *val[i1] = (m.value(i0, i1) - m.value(i1, i0)) * S;
//    *val[i2] = (m.value(i0, i2) - m.value(i2, i0)) * S;
//}


Matrix4 Quaternion::toRotationMatrix() const
{
    return Matrix4(toRotationMatrix3());
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

Matrix3 Quaternion::toRotationMatrix3() const
{
    Matrix3 r;

    REAL t2X = 2 * m_x;
    REAL t2Y = 2 * m_y;
    REAL t2Z = 2 * m_z;

    REAL t2XW = t2X * m_w;
    REAL t2YW = t2Y * m_w;
    REAL t2ZW = t2Z * m_w;
    REAL t2XX = t2X * m_x;
    REAL t2XY = t2Y * m_x;
    REAL t2XZ = t2Z * m_x;
    REAL t2YY = t2Y * m_y;
    REAL t2YZ = t2Z * m_y;
    REAL t2ZZ = t2Z * m_z;

    r.value(0, 0) = 1 - t2YY - t2ZZ;
    r.value(0, 1) = t2XY - t2ZW;
    r.value(0, 2) = t2XZ + t2YW;
    r.value(1, 0) = t2XY + t2ZW;
    r.value(1, 1) = 1 - t2XX - t2ZZ;
    r.value(1, 2) = t2YZ - t2XW;
    r.value(2, 0) = t2XZ - t2YW;
    r.value(2, 1) = t2YZ + t2XW;
    r.value(2, 2) = 1 - t2XX - t2YY;

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

//????????????????????????????????????????????????????????
//Vector3 Quaternion::operator *(const Vector3& v) const
//{
//    Vector3 zQVec(m_x, m_y, m_z);
//    Vector3 zUV(zQVec & v);
//    Vector3 zUUV(zQVec & zUV);

//    zUV *= (2 * m_w);
//    zUUV *= 2;

//    return v + zUV + zUUV;
//}

/*
Рысканье = heading = yaw = вокруг оси Z; тангаж = altitude = pitch = вокруг оси Y; крен = bank = roll = вокруг оси X.
*/


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
