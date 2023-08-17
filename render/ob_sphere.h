#pragma once

#include <vector>

#include "defines.h"
#include "matrix3.h"
#include "ray.h"

namespace Render
{

class Triangle;

class ObSphere
{
public:
    ObSphere() = default;
    ObSphere(const ObSphere& obs) { *this = obs; }
    ObSphere(const Vector3& v, REAL r) { m_center = v; setRadius(r); }
    virtual ~ObSphere() = default;

    Vector3& center() { return m_center; }
    void setRadius(REAL r) { m_radius = r; m_radius_2 = r * r; }
//    REAL& radius() { return m_radius; }

    const Vector3& center() const { return m_center; }
    const REAL radius() const { return m_radius; }

    ObSphere& operator =(const ObSphere& obs) { m_center = obs.center(); setRadius(obs.radius()); return *this; }
//    ObSphere& operator +=(const ObSphere& zSphere);

    void compute(const std::vector<Vector3>& azData);
    void compute(const Triangle& t);

    ObSphere transform(const Matrix3& zRotate, const Vector3& zTranslate, REAL fScale)
    {
        ObSphere obs;
        obs.center() = fScale * (zRotate * m_center) + zTranslate;
        obs.setRadius(fScale * m_radius);
        return obs;
    }

    bool intersect(const Ray& r) const;

private:
    Vector3 m_center;
    REAL m_radius;
    REAL m_radius_2;
};

//ZONE_INLINE Sphere& Sphere::operator +=(const Sphere& zSphere)
//{
//	static const float cs_fTolerance = 1e-6f;

//	Vector zDiff = f_zCenter - zSphere.f_zCenter;

//	float fLengthSqr = zDiff.SquaredLength();
//	float fDeltaRad = zSphere.f_fRadius - f_fRadius;
//	float fDeltaRadSqr = fDeltaRad*fDeltaRad;

//	float fLength;
//	float fAlpha;

//	if(fDeltaRad >= 0)
//	{
//		if(fDeltaRadSqr >= fLengthSqr)
//		{
//			f_zCenter = zSphere.f_zCenter;
//			f_fRadius = zSphere.f_fRadius;
//		}
//		else
//		{
//			if((fLength = sqrtf(fLengthSqr)) > cs_fTolerance)
//			{
//				fAlpha = (fLength - fDeltaRad)/(2*fLength);
//				f_zCenter = zSphere.f_zCenter + fAlpha*zDiff;
//			}
//			f_fRadius = 0.5f*(zSphere.f_fRadius + fLength + f_fRadius);
//		}
//	}
//	else if(fDeltaRadSqr < fLengthSqr)
//	{
//		if((fLength = sqrtf(fLengthSqr)) > cs_fTolerance)
//		{
//			fAlpha = (fLength - fDeltaRad)/(2*fLength);
//			f_zCenter = zSphere.f_zCenter + fAlpha*zDiff;
//		}
//		f_fRadius = 0.5f*(zSphere.f_fRadius + fLength + f_fRadius);
//	}

//	return *this;
//}

} // namespace Render
