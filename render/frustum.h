#pragma once

#include "plane.h"
#include "matrix4.h"
//#include "quaternion.h"

namespace Render
{

const REAL c_InfiniteFarPlaneAdjust = REAL(0.00001);

class Frustum
{
public:
    enum class ProjectionType
	{
        Ortho,
        Perspective
	};

	enum FrustumPlane
	{
		Frustum_Near,
		Frustum_Far,
		Frustum_Left,
		Frustum_Right,
		Frustum_Top,
		Frustum_Bottom
	};

	Frustum();
    virtual ~Frustum() = default;

    REAL& fov() { m_recalcFrustum = true; return m_fov; }
    REAL& nearClip() { m_recalcFrustum = true; return m_near; }
    REAL& farClip() { m_recalcFrustum = true; return m_far; }
    REAL& aspect() { m_recalcFrustum = true; return m_aspect; }
    ProjectionType& type() { m_recalcFrustum = true; return m_type; }

    REAL fov() const { return m_fov; }
    REAL nearClip() const { return m_near; }
    REAL farClip() const { return m_far; }
    REAL aspect() const { return m_aspect; }
    const ProjectionType Type() const { return m_type; }

    REAL thetaY() const { return m_thetaY; }
    REAL tanThetaY() const { return m_tanThetaY; }
    REAL tanThetaX() const { return m_tanThetaX; }

    const Matrix4& projection() const { return m_projection; }
    const Matrix4& stdProjection() const { return m_stdProjection; }
    const Matrix4& view() const { return m_view; }
    const Matrix4& look() const { return m_look; }

    const Plane& plane(int i) const { return m_plane[i]; }

    bool isVisible(const Vector3 &zVertex, FrustumPlane* fp = 0) const;
    //bool isVisible(const Box3& b, FrustumPlane *fp = 0) const;
    //bool isVisible(const Sphere &zBound, FrustumPlane *pzCulledBy = 0) const;

    const Vector3* worldSpace() const { return m_worldCorner; }

    void enableReflection(const Plane& p) { m_isReflect = true; m_reflectPlane = p; m_reflect.makeReflection(m_reflectPlane); m_recalcView = true; }
    void disableReflection() { m_isReflect = false; m_recalcView = true; }
    bool isReflected() const { return m_isReflect; }

    const Matrix4& reflection() const { return m_reflect; }
    const Plane& reflectionPlane() const { return m_reflectPlane; }

    void update();

private:
    ProjectionType m_type;

    REAL m_fov;
    REAL m_near;
    REAL m_far;
    REAL m_aspect;

    Plane m_plane[6];

    Matrix4 m_projection;
    Matrix4 m_stdProjection;
    Matrix4 m_view;
    Matrix4 m_look;

    bool m_recalcFrustum = true;
    bool m_recalcView = true;

    REAL m_coeffL[2];
    REAL m_coeffR[2];
    REAL m_coeffB[2];
    REAL m_coeffT[2];

    bool m_isReflect;
    Matrix4 m_reflect;
    Plane m_reflectPlane;

    Vector3 m_worldCorner[8];

    // precalculated
    REAL m_thetaY;
    REAL m_tanThetaY;
    REAL m_tanThetaX;

    void updateView();
    void updateFrustum();

    bool isViewChanged();
};

//namespace Render
}
