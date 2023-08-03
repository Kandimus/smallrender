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

bool loadVectorOfVec3(std::vector<Render::Vector3>& vec3, int accIndex, const tinygltf::Model& model);
bool loadVectorOfVec2(std::vector<Render::Vector2>& vec2, int accIndex, const tinygltf::Model& model);
bool loadVectorOfInt(std::vector<int>& veci, int accIndex, const tinygltf::Model& model);

//namespace Render
}
