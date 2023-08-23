#pragma once

#include <vector>

#include "i_object.h"

namespace Render
{

class Vector3;

class IObVolume : public IObject
{
public:
    IObVolume() = default;
    virtual ~IObVolume() = default;

    virtual bool in(const Vector3& point) const = 0;
    virtual void create(const std::vector<Vector3>& data) = 0;
};

}
