#pragma once

#include <vector>

#include "Vector2.h"
#include "Vector3.h"

namespace tinygltf
{
class Model;
}

namespace Render
{

// The MAGIC number for covertion light power from blender W to i
static const REAL light_W_to_i = 35.0;

bool loadVectorOfVec3(std::vector<Render::Vector3>& vec3, int accIndex, const tinygltf::Model& model);
bool loadVectorOfVec2(std::vector<Render::Vector2>& vec2, int accIndex, const tinygltf::Model& model);
bool loadVectorOfInt(std::vector<int>& veci, int accIndex, const tinygltf::Model& model);

//namespace Render
}
