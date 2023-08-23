
#include "render.h"
#include "camera.h"
#include "frustum.h"

namespace Render
{

Frustum::Frustum()
{
    m_fov = REAL(M_PI_2) / 2;
    m_near = REAL(100);
    m_far = REAL(100000);
    m_aspect = REAL(1.3333333333333);
    m_type = ProjectionType::Perspective;

    m_recalcFrustum = m_recalcView = true;

    m_view = m_look = m_projection = m_reflect = Matrix4::c0;

    m_isReflect = false;
}

//bool Frustum::isVisible(const Box3& b, FrustumPlane *fp) const
//{
//    //updateView();
//    std::vector<Vector3> azCorner(8);
//    b.compute(azCorner);

//    for (int iPlane = 0; iPlane <= 6; ++iPlane)
//    {
//        if (0 == m_far && Frustum_Far == iPlane)
//        {
//            continue;
//        }

//        if (Plane::Side_Negative == m_plane[iPlane].side(azCorner[0]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[1]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[2]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[3]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[4]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[5]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[6]) &&
//            Plane::Side_Negative == m_plane[iPlane].side(azCorner[7]))
//        {
//            if(fp)
//            {
//                *fp = static_cast<FrustumPlane>(iPlane);
//            }
//            return false;
//        }
//    }
//    return true;
//}

//bool Frustum::IsVisible(const Sphere &zBound, FrustumPlane *pzCulledBy) const
//{
//ZONE_B("Frustum::IsVisible");

//	UpdateView();
//	for(int iPlane = 0; iPlane <= 6; ++iPlane)
//	{
//		if(0.0f == f_fFar && Frustum_Far == iPlane)
//		{
//			continue;
//		}

//		if(f_azPlane[iPlane].DistanceToPoint(zBound.Center()) < -zBound.Radius())
//		{
//			if(pzCulledBy)
//			{
//				*pzCulledBy = static_cast<FrustumPlane>(iPlane);
//			}
//			return false;
//		}
//	}
//	return true;
//}

bool Frustum::isVisible(const Vector3& v, FrustumPlane *fp) const
{
//    updateView();

	for(int iPlane = 0; iPlane <= 6; ++iPlane)
	{
        if(0.0 == m_far && Frustum_Far == iPlane)
		{
			continue;
		}

        if(Plane::Side_Negative == m_plane[iPlane].side(v))
		{
            if(fp)
			{
                *fp = static_cast<FrustumPlane>(iPlane);
			}
			return false;
		}
	}
    return true;
}

void Frustum::update()
{
    updateView();
    updateFrustum();
}

void Frustum::updateView()
{
    if(!isViewChanged())
	{
		return;
	}

    const Vector3 cam_p = Camera::instance().position();
    const Vector3 cam_d = Camera::instance().direction();
    const Vector3 cam_u = Camera::instance().up();

    m_view.view(cam_p, cam_p + cam_d, cam_u);
    m_look.view(0, cam_d, cam_u);

    if (m_isReflect)
	{
        m_view *= m_reflect;
        m_look *= m_reflect;
	}

    updateFrustum();

//	Vector3& zLeft = Camera::GetPointer()->GetLeft();
//	Vector3& zUp = Camera::GetPointer()->GetUp();
//	Vector3& zPosition = Camera::GetPointer()->Position();

	//Vector zCamDirection = Camera::GetPointer()->GetDirection();
    Quaternion q;
    q.fromYPR(-Camera::instance().angleX(), -Camera::instance().angleY(), 0);
    auto q_dir = q.direction();
    REAL fDdE = q_dir & cam_p;


    Vector3 zNormal(m_coeffL[0] * q_dir + m_coeffL[1] * q_dir);
    m_plane[Frustum_Left].normal() = zNormal;
    m_plane[Frustum_Left].constant() = cam_p & zNormal;

    zNormal = m_coeffR[0] * q_dir + m_coeffR[1] * q_dir;
    m_plane[Frustum_Right].normal() = zNormal;
    m_plane[Frustum_Right].constant() = cam_p & zNormal;

    zNormal = m_coeffB[0] * Vector3::cY + m_coeffB[1] * q_dir;
    m_plane[Frustum_Bottom].normal() = zNormal;
    m_plane[Frustum_Bottom].constant() = cam_p & zNormal;

    zNormal = m_coeffT[0] * Vector3::cY + m_coeffT[1] * q_dir;
    m_plane[Frustum_Top].normal() = zNormal;
    m_plane[Frustum_Top].constant() = cam_p & zNormal;

    m_plane[Frustum_Far].normal() = -q_dir;
    m_plane[Frustum_Far].constant() = -fDdE - m_far;

    m_plane[Frustum_Near].normal() = q_dir;
    m_plane[Frustum_Near].constant() = fDdE + m_near;

    Matrix4 zEye = m_view.inverse();

    REAL fFarDist = (0 == m_far) ? REAL(100000) : m_far;
    REAL fNearY = m_tanThetaY * m_near;
    REAL fFarY = m_tanThetaY * ((ProjectionType::Ortho == m_type)? m_near: fFarDist);
    REAL fNearX = m_tanThetaX * m_near;
    REAL fFarX = m_tanThetaX * ((ProjectionType::Ortho == m_type)? m_near: fFarDist);

    m_worldCorner[0] = zEye * Vector3( fNearX,  fNearY, -m_near);
    m_worldCorner[1] = zEye * Vector3(-fNearX,  fNearY, -m_near);
    m_worldCorner[2] = zEye * Vector3(-fNearX, -fNearY, -m_near);
    m_worldCorner[3] = zEye * Vector3( fNearX, -fNearY, -m_near);

    m_worldCorner[4] = zEye * Vector3( fFarX,  fFarY, -fFarDist);
    m_worldCorner[5] = zEye * Vector3(-fFarX,  fFarY, -fFarDist);
    m_worldCorner[6] = zEye * Vector3(-fFarX, -fFarY, -fFarDist);
    m_worldCorner[7] = zEye * Vector3( fFarX, -fFarY, -fFarDist);

    if (m_isReflect)
	{
        Vector3 zPos(m_reflect * cam_p);
        Vector3 zDir(q_dir.reflect(m_reflectPlane.normal()));
		fDdE = zDir & zPos;

        for (int i = 0; i < 6; ++i)
		{
            m_plane[i].normal() = m_plane[i].normal().reflect(m_reflectPlane.normal());

			switch(i)
			{
				case Frustum_Near:
                    m_plane[i].constant() = -(fDdE + m_near);
					break;

				case Frustum_Far:
                    m_plane[i].constant() = fDdE + m_far;
					break;

				default:
                    m_plane[i].constant() = -(zPos & m_plane[i].normal());
					break;
			}
		}

        for(int i = 0; i < 8; ++i)
		{
            m_worldCorner[i] = m_reflect * m_worldCorner[i];
		}
	}

    m_recalcView = false;
}

void Frustum::updateFrustum()
{
    if (!m_recalcFrustum)
	{
		return;
	}

    m_thetaY = REAL(0.5) * m_fov;
    m_tanThetaY = TAN(m_thetaY);
    m_tanThetaX = m_tanThetaY * m_aspect;
    REAL fVPTop = m_tanThetaY * m_near;
    REAL fVPRight = m_tanThetaX * m_near;
    REAL fVPBottom = -fVPTop;
    REAL fVPLeft = -fVPRight;
    REAL fNSqr = m_near * m_near;
    REAL fLSqr = fVPRight * fVPRight;
    REAL fRSqr = fLSqr;
    REAL fTSqr = fVPTop * fVPTop;
    REAL fBSqr = fTSqr;

    if(ProjectionType::Perspective == m_type)
	{
        Render::makeProjection(m_fov, m_aspect, m_near, m_far, m_projection);
        Render::makeProjection(m_fov, m_aspect, m_near, m_far, m_stdProjection/*, true*/);
	}
	else
	{
        Render::makeOrtho(m_fov, m_aspect, m_near, m_far, m_projection);
        Render::makeOrtho(m_fov, m_aspect, m_near, m_far, m_stdProjection/*, true*/);
	}

    REAL invLength = REAL(1)/ SQRT(fNSqr + fLSqr);
    m_coeffL[0] = m_near * invLength;
    m_coeffL[1] = -fVPLeft * invLength;

    invLength = REAL(1) / SQRT(fNSqr + fRSqr);
    m_coeffR[0] = -m_near * invLength;
    m_coeffR[1] = fVPRight * invLength;

    invLength = REAL(1) / SQRT(fNSqr + fBSqr);
    m_coeffB[0] = m_near * invLength;
    m_coeffB[1] = -fVPBottom * invLength;

    invLength = 1.0f/sqrtf(fNSqr + fTSqr);
    m_coeffT[0] = -m_near * invLength;
    m_coeffT[1] = fVPTop * invLength;

    m_recalcFrustum = false;
}

//-------------------------------------------------------------------------------------------------

bool Frustum::isViewChanged()
{
    if (camera().isChanged())
	{
        return true;
    }

    return m_recalcView;
}

//namespace Render
}
