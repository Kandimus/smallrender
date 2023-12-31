#pragma once

#include <vector>
#include <string>

#include "i_object.h"

namespace Render
{

class Vector3;

class IObVolume : public IObject
{
public:
    IObVolume() = default;
    virtual ~IObVolume() = default;

    virtual std::string type() const = 0;
    virtual bool in(const Vector3& point) const = 0;
    virtual void create(const std::vector<Vector3>& data) = 0;
    virtual Vector3 minPoint() const = 0;
    virtual Vector3 maxPoint() const = 0;
};

}
