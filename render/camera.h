#pragma once

#include <string>

#include "i_object.h"
#include "ray.h"
#include "quaternion.h"
#include "frustum.h"
#include "vector2.h"

namespace tinygltf
{
struct Node;
class Model;
}

namespace Render
{

class Camera  : public IObject //TODO Переделать или на UVN вектора (как в Vers) или на кватернионы
{
public:
    virtual ~Camera() = default;
    static Camera& instance() { static Camera singleton; return singleton; }

private:
    Camera() { }
    Camera(const Camera&);
    Camera& operator=(Camera&);

public:
    Vector3& position() { m_isChangedPosition = true; return m_position; }
//    REAL& angleX() { m_isChangedDirection = true; return m_angleX; }
//    REAL& angleY() { m_isChangedDirection = true; return m_angleY; }
//    REAL& moveSpeed() { return m_moveSpeed; }
//    REAL& rotateSpeed() { return m_rotateSpeed; }
    Frustum& frustum() { return m_frustum; }

//    REAL angleX() const { return m_angleX; }
//    REAL angleY() const { return m_angleY; }
//    REAL moveSpeed() const { return m_moveSpeed; }
//    REAL rotateSpeed() const { return m_rotateSpeed; }
    const Frustum& frustum() const { return m_frustum; }

    std::string& name() { return m_name; }
    const std::string& name() const { return m_name; }

    //const Quaternion& orientation() const { return m_orientation; }
    const Vector3& position() const { return m_position; }
    const Vector3& direction() const { return m_direction; }
    const Vector3& right() const { return m_right; }
    const Vector3& up() const { return m_up; }

    void setViewport(REAL left, REAL right, REAL top, REAL bottom) { m_portLeft = left; m_portRight = right; m_portTop = top; m_portBottom = bottom; }
    void setViewport();

    Ray ray(int screen_x, int screen_y) const;
    Ray centralRay() const;

//    void MoveForward(float fRun);//m_isChangedPosition = true
//    void MoveBackward(float fRun);//m_isChangedPosition = true
//    void MoveUp(float fRun);//m_isChangedPosition = true
//    void MoveDown(float fRun);//m_isChangedPosition = true
//    void StrafeLeft(float fRun);//m_isChangedPosition = true
//    void StrafeRight(float fRun);//m_isChangedPosition = true
//    void TurnLeft(const int); //m_isChangedDirection = true
//    void TurnRight(const int); //m_isChangedDirection = true
//    void LookUp(const int);//m_isChangedDirection = true
//    void LookDown(const int);//m_isChangedDirection = true
    void changed() {  m_isChangedPosition = m_isChangedDirection = true; }
    bool isChanged() const {  return m_isChangedPosition || m_isChangedDirection; }
    void update();
    void reset();

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

    // IObject
    virtual bool intersect(const Ray& ray) const override { return false; }
    virtual void tranformation(const Matrix4& m4) override
    {
        m_position = m_position * m4;

        Matrix4 mr = m4.clearTranslate();
        m_direction = m_direction * mr;
        m_up = m_up * mr;
        m_right = m_right * mr;

        m_direction.normalize();
        m_up.normalize();
        m_right.normalize();

        //m_direction = -m_direction;
    }

private:
    void updatePosition()
    {
        if (!m_isChangedPosition)
        {
            return;
        }
    }

    void updateDirection()
    {
        if (!m_isChangedDirection)
        {
            return;
        }
    }

protected:
    std::string m_name = "camera";

    Vector3 m_position;
    Vector3 m_direction;
    Vector3 m_up;
    Vector3 m_right;

    REAL m_moveSpeed;
    REAL m_rotateSpeed;

    REAL m_portLeft;
    REAL m_portRight;
    REAL m_portTop;
    REAL m_portBottom;

    REAL m_viewX;
    REAL m_viewY;
    REAL m_viewW;
    REAL m_viewH;

    bool m_isChangedPosition;
    bool m_isChangedDirection;

    Frustum m_frustum;

    //precalculated
    Quaternion m_orientation;
};

}
