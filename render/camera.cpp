
#include <iostream>

#include "camera.h"
#include "render.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"


namespace
{
std::string PERSPECTIVE = "perspective";
}

namespace Render
{

void Camera::reset()
{
    m_position = Vector3::c0;
    m_angleX = 0;
    //m_angleX = M_PI;
    m_angleY = 0;
    //m_angleY = M_PI;

    m_frustum.fov() = REAL(M_PI_2);
    m_frustum.nearClip() = REAL(1);
    m_frustum.farClip() = REAL(52);
    m_frustum.aspect() = REAL(1);
    //	SetFrustum(1, 48, -1.0f, 1.0f, 1.0f, -1.0f);

    m_portLeft = m_portBottom = 0;
    m_portRight = m_portTop = 1;

    //    f_fMoveSpeed = f_fRotateSpeed = 0.01f;

    //    f_zBSphere.Radius() = 3.0f;

    m_isChangedPosition = true;
    m_isChangedDirection = true;

    update();
}

void Camera::setViewport()
{
    int w = Render::image_width();
    int h = Render::image_height();

    m_viewX = m_portLeft * w;
    m_viewY = m_portBottom * h;
    m_viewW = (m_portRight - m_portLeft) * w;
    m_viewH = (m_portTop - m_portBottom) * h;

//    if(Renderer::c_ModeD3D == Renderer::GetPointer()->GetMode())
//    {
//        D3DVIEWPORT9 zViewport;
//        zViewport.X = (DWORD)fX;
//        zViewport.Y = (DWORD)fY;
//        zViewport.Width = (DWORD)fWidth;
//        zViewport.Height = (DWORD)fHeight;
//        zViewport.MinZ = 0.0f;
//        zViewport.MaxZ = 1.0f;

//        Renderer::GetPointer()->GetDevice()->SetViewport(&zViewport);
//    }
//    else
//    {
//        GLint   iX      = (GLint)(fX);
//        GLint   iY      = (GLint)(fY);
//        GLsizei iWidth  = (GLsizei)(fWidth);
//        GLsizei iHeight = (GLsizei)(fHeight);

//        glViewport(iX, iY, iWidth, iHeight);
//        if(GLenum iError = glGetError())
//        {
//            ZONE_THROW_E("glViewport, code: " + ToStringGL(iError));
//        }

//        glDepthRange(0.0f, 1.0f);
//        if(GLenum iError = glGetError())
//        {
//            ZONE_THROW_E("glDepthRange, code: " + ToStringGL(iError));
//        }
//    }
}

Ray Camera::ray(int screen_x, int screen_y) const
{
    REAL iWidth = Render::image_width();
    REAL iHeight = Render::image_height();

    REAL fPortX = (iWidth - 1 - screen_x) / iWidth;
    REAL fPortY = (iHeight - 1 - screen_y) / iHeight;

    if (fPortX < m_portLeft || fPortX > m_portRight)
    {
        return Ray(Vector3::c0, Vector3::c0);
    }

    if (fPortY < m_portBottom || fPortY > m_portTop)
    {
        return Ray(Vector3::c0, Vector3::c0);
    }

    //TODO оптимизировать, много статичных вычислений
    REAL fVPTop = m_frustum.tanThetaY() * m_frustum.nearClip();
    REAL fVPRight = m_frustum.tanThetaX() * m_frustum.nearClip();
    REAL fVPBottom = -fVPTop;
    REAL fVPLeft = -fVPRight;

    REAL fViewX = (1 - fPortX) * fVPLeft + fPortX * fVPRight;
    REAL fViewY = (1 - fPortY) * fVPBottom + fPortY * fVPTop;

    Vector3 d = m_frustum.nearClip() * direction() + fViewX * left() + fViewY * up();
    d.normalize();

    return Ray(m_position, d);
}

Ray Camera::centralRay() const
{
    return Ray(m_position, direction());
}

void Camera::update()
{
    updateDirection();
    updatePosition();

    m_frustum.update();

    m_isChangedPosition = false;
    m_isChangedDirection = false;
}

bool Camera::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{
    if (node.camera < 0 || model.cameras.size() <= node.camera)
    {
        return false;
    }

    auto& camera = model.cameras[node.camera];
    m_name = camera.name;

    if (camera.type == ::PERSPECTIVE)
    {
        m_position = loadNodeTranslation(node);

        auto q = loadNodeRotation(node);

        Quaternion qa;
        qa.x() = -q.z();
        qa.y() = -q.x();
        qa.z() = q.y();
        qa.w() = q.w();

        auto a = qa.toYPR();

        Render::camera().angleX() = -a.z();
        Render::camera().angleY() = a.y();

//        m_angleX = -35 * MATH_PI / 180;
//        m_angleY = (90 - 70) * MATH_PI / 180;

//        Quaternion ypr;

//        ypr.fromYPR(-m_angleX, m_angleY, 0);
//        std::cout << "camera ypr " << ypr.toString() << ", AngleX " << m_angleX << ", AngleY " << m_angleY << std::endl;

        m_frustum.aspect() = camera.perspective.aspectRatio;
        m_frustum.fov() = camera.perspective.yfov;
        m_frustum.farClip() = camera.perspective.zfar;
        m_frustum.nearClip() = camera.perspective.znear;
        changed();
        update();

        return true;
    }

    return false;
}

// namespace Render
}
