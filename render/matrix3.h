#pragma once

#include <memory.h>

#include "defines.h"
#include "vector3.h"

namespace Render
{

class Matrix3
{
public:
	static const Matrix3 c0;
	static const Matrix3 c1;

    Matrix3() { reset(); }
    Matrix3(REAL f00, REAL f01, REAL f02, REAL f10, REAL f11, REAL f12, REAL f20, REAL f21, REAL f22)
    {
        m_00 = f00; m_01 = f01; m_02 = f02;
        m_10 = f10; m_11 = f11; m_12 = f12;
        m_20 = f20; m_21 = f21; m_22 = f22; }

    void makeAxisAngle(const Vector3& axis, REAL angle);
    void makeRotateY(REAL fAngle);

    void makeEuler(REAL fYaw, REAL fPitch, REAL fRoll);
    void toEuler(REAL& aX, REAL& aY, REAL& aZ) const;

    void reset() { memset(m_value, 0, sizeof(REAL) * 9); }

    REAL& value(int i) { return m_value[i]; }
    REAL value(int i) const { return m_value[i]; }
    REAL& value(int row, int col) { return m_matrix[row][col]; }
    REAL value(int row, int col) const { return m_matrix[row][col]; }

    Vector3 column(int col) const { return Vector3(m_matrix[0][col], m_matrix[1][col], m_matrix[2][col]); }

    Matrix3 operator -() const { Matrix3 r; for(int ii = 0; ii < 9; ++ii) r.value(ii) = -value(ii); return r; }

    Matrix3 operator *(const Matrix3& zM) const;
    Vector3 operator *(const Vector3& zV) const;

    Matrix3 transpose() const;

private:
	union
	{
		struct
		{
            REAL m_00, m_01, m_02;
            REAL m_10, m_11, m_12;
            REAL m_20, m_21, m_22;
		};
        REAL m_matrix[3][3];
        REAL m_value[9];
	};
};

// namespace Render
}
