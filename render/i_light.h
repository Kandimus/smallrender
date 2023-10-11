#pragma once

#include <string>

namespace Render
{

class Vector3;
class Ray;

enum class LightType
{
    AMBIENT,
    DIRECTIONAL,
    POINT,
    SPOT,
};

class ILight
{
public:
    ILight() = default;
    virtual ~ILight() = default;

    virtual const std::string& name() const = 0 ;
    virtual void setName(const std::string& name) = 0;

    virtual bool enable() const = 0;
    virtual bool& enable() = 0;

    virtual LightType type() const = 0;
    virtual Ray ray(const Vector3& p) const = 0;
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const = 0;
    virtual std::string toString() const = 0;
};

} //namespace Render
