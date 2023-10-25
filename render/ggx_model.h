#pragma once

#include "vector3.h"

namespace Render
{
class Material;

namespace GGX
{

Vector3 cookTorrance(Vector3 n, Vector3 l, Vector3 v, const Material& m);

} //namespace GGX
} //namespace Render
