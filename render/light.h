#pragma once

#include <string>

#include "vector3.h"
#include "ray.h"
#include "color_rgb.h"

namespace tinygltf
{
struct Node;
class Model;
}


namespace Render
{

class Light
{
public:
    enum class Type
    {
        AMBIENT,
        DIRECTIONAL,
        POINT,
        SPOT,
    };

    Light() { }
    virtual ~Light() = default;

    void createAmbient(const ColorRGB& ambient)
    {
        m_type = Type::AMBIENT;
        m_ambient = ambient;

        m_enable = true;
    }

    void createDirectional(const Vector3& direction, const Vector3& diffuse, const Vector3& specular, const ColorRGB& color)
    {
        m_type = Type::DIRECTIONAL;
        m_direction = direction;
        m_diffuse   = diffuse;
        m_specular  = specular;
        m_color = color;

        m_enable = true;
	}

    void createPoint(const Vector3& position, REAL range, const Vector3& attenuation, const Vector3& diffuse, const Vector3& specular, const ColorRGB& color)
    {
        m_type = Type::POINT;
        m_position = position;
        m_range = range;
        m_attenuation = attenuation;
        m_diffuse = diffuse;
        m_specular = specular;
        m_color = color;

        m_enable = true;
    }

    void createSpot(const Vector3& position, REAL range, REAL angle, const Vector3& direction, const Vector3& diffuse, const Vector3& specular, const ColorRGB& color)
    {
        m_type = Type::SPOT;
        m_position = position;
        m_range = range;
        m_angle = angle;
        m_direction = direction;
        m_diffuse = diffuse;
        m_specular = specular;
        m_color = color;

        m_enable = true;
    }

    const std::string& name() const { return m_name; }
    std::string& name() { return m_name; }

    Type type() const { return m_type; }
    Type& type() { return m_type; }

    const ColorRGB& ambient() const { return m_ambient; }
    ColorRGB& ambient() { return m_ambient; }

    const Vector3& position() const { return m_position; }
    Vector3& position() { return m_position; }

    const Vector3& direction() const { return m_direction; }
    Vector3& direction() { return m_direction; }

    const Vector3& diffuse() const { return m_diffuse; }
    Vector3& diffuse() { return m_diffuse; }

    const Vector3& specular() const { return m_specular; }
    Vector3& specular() { return m_specular; }

    const Vector3& attenuation() const { return m_attenuation; }
    Vector3& attenuation() { return m_attenuation; }

    REAL range() const { return m_range; }
    REAL& range() { return m_range; }

    REAL angle() const { return m_angle; }
    REAL& angle() { return m_angle; }

    bool enable() const { return m_enable; }
    bool& enable() { return m_enable; }

    // Может ли влиять источник света на точку
    //TODO Может стоит перейти на класс-интерфейс и конкретные реализации? Так как реализация возможности влияние спота не так проста!
    inline bool isAffects(const Vector3& point) { return m_enable ? ((m_type == Type::SPOT || m_type == Type::POINT) ? (m_position - point).length() < m_range : true) : false;  }

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

private:
    std::string m_name = "";
    Type m_type = Type::AMBIENT;
    Vector3 m_position = Vector3::c0;
    Vector3 m_direction = Vector3::c0;
    Vector3 m_diffuse = Vector3::c0; // рассеянный свет
    Vector3 m_specular = Vector3::c0; // отраженный свет
    ColorRGB m_ambient = ColorRGB::White; // цвет света :) ambient
    Vector3 m_attenuation = Vector3::c0; // затухание x - ks, y - kl, z - kq
    REAL m_angle = MATH_PI_4;
    REAL m_range = 0;

    bool m_enable = false;


    static ColorRGB m_globalColor;
};

} //namespace Render
