
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
    m_position  = Vector3::c0;
    m_direction = -Vector3::cZ;
    m_right     = Vector3::cX;
    m_up        = Vector3::cY;

    m_frustum.fov() = REAL(M_PI_2);
    m_frustum.nearClip() = REAL(1);
    m_frustum.farClip() = REAL(52);
    m_frustum.aspect() = REAL(1);

    m_portLeft = m_portBottom = 0;
    m_portRight = m_portTop = 1;

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
}

Ray Camera::ray(int screen_x, int screen_y) const
{
    if (screen_x < 0 || screen_x >= Render::image_width() ||
        screen_y < 0 || screen_y >= Render::image_height())
    {
        return Ray(Vector3::c0, Vector3::c0);
    }

    REAL w = Render::image_width();
    REAL h = Render::image_height();
    REAL fPortX = (w - 1.0 - screen_x) / w;
    REAL fPortY = (h - 1.0 - screen_y) / h;

    //TODO оптимизировать, много статичных вычислений
    REAL fVPTop = m_frustum.tanThetaY() * m_frustum.nearClip();
    REAL fVPRight = m_frustum.tanThetaX() * m_frustum.nearClip();

    REAL fViewX = (1 - fPortX) * -fVPRight + fPortX * fVPRight;
    REAL fViewY = (1 - fPortY) * -fVPTop + fPortY * fVPTop;

    Vector3 d = m_frustum.nearClip() * direction() - fViewX * right() + fViewY * up();
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

    reset();

    auto& camera = model.cameras[node.camera];
    m_name = camera.name;

    if (camera.type == ::PERSPECTIVE)
    {
        Matrix4 m4 = loadNodeTransformationMatrix(node);
        tranformation(m4);

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
