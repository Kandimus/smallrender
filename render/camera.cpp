
#include <iostream>

#include "camera.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace
{
std::string PERSPECTIVE = "perspective";
}

namespace Render
{

class CameraInternal
{
public:
    REAL thetaY;
    REAL tanThetaY;
    REAL tanThetaX;
    REAL vewportTop;
    REAL vewportRight;
};

Camera::Camera()
{
    d = new CameraInternal();

    reset();
}

Camera::~Camera()
{
    delete d;
}

void Camera::reset()
{
    m_position  = Vector3::c0;
    m_direction = Vector3::cZ;
    m_right     = Vector3::cX;
    m_up        = Vector3::cY;

    m_fov = REAL(M_PI_2);
    m_near = REAL(1);
    m_far = REAL(100);
    m_aspect = REAL(1.3333333333333);

    m_portLeft = m_portBottom = 0;
    m_portRight = m_portTop = 1;

    setChanged();
    update();
}

void Camera::setViewport(REAL left, REAL right, REAL top, REAL bottom)
{
    m_portLeft = left;
    m_portRight = right;
    m_portTop = top;
    m_portBottom = bottom;

    setChanged();
}

void Camera::setViewport(int h)
{
    m_width = h * m_aspect;
    m_height = h;

    m_portLeft = 0.0;
    m_portRight = 1.0;
    m_portBottom = 0.0;
    m_portTop = 1.0;

    setChanged();
}

void Camera::setScreen(int h)
{
    m_width = h * m_aspect;
    m_height = h;

    setChanged();
}

Ray Camera::ray(int screen_x, int screen_y) const
{
    if (screen_x < 0 || screen_x >= m_width ||
        screen_y < 0 || screen_y >= m_height)
    {
        return Ray(Vector3::c0, Vector3::c0);
    }

    REAL portX = (m_width - 1.0 - screen_x) / m_width;
    REAL portY = (m_height - 1.0 - screen_y) / m_height;
    REAL fViewX = (1 - portX) * -d->vewportRight + portX * d->vewportRight;
    REAL fViewY = (1 - portY) * -d->vewportTop + portY * d->vewportTop;

    Vector3 d = m_near * direction() - fViewX * right() + fViewY * up();
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

    // frustum
    d->thetaY = REAL(0.5) * m_fov;
    d->tanThetaY = TAN(d->thetaY);
    d->tanThetaX = d->tanThetaY * m_aspect;
    d->vewportTop = d->tanThetaY * m_near;
    d->vewportRight = d->tanThetaX * m_near;

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
        Matrix4 m4 = loadNodeTransformationMatrix(node);
        tranformation(m4);

        m_aspect = camera.perspective.aspectRatio;
        m_fov = camera.perspective.yfov;
        m_far = camera.perspective.zfar;
        m_near = camera.perspective.znear;

        setChanged();
        update();

        return true;
    }

    return false;
}

// IObject
void Camera::tranformation(const Matrix4& m4)
{
    m_position = m4 * m_position;

    Matrix4 mr = m4.clearTranslate();
    m_direction = mr * m_direction;
    m_up = mr * m_up;
    m_right = mr * m_right;

    m_direction.normalize();
    m_up.normalize();
    m_right.normalize();

    //m_direction = -m_direction;
}

std::string Camera::toString() const
{
    return "{name: '" + m_name + "', pos: " + m_position.toString() + ", direction: " + m_direction.toString() +
           ", up: " + m_up.toString() + ", right: " + m_right.toString() + "}";
}

// namespace Render
}
