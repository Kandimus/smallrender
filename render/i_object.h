#pragma once

#include <string>

namespace Render
{

class Ray;
class Matrix4;

class IObject
{
public:
    IObject() = default;
    virtual ~IObject() = default;

    virtual bool intersect(const Ray& ray) const = 0;
    virtual void tranformation(const Matrix4& m4) = 0;

    virtual std::string toString() const = 0;
};

}
