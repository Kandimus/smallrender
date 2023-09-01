#pragma once

#include <vector>

#include "Vector2.h"
#include "Vector3.h"
#include "quaternion.h"
#include "color_argb.h"

namespace tinygltf
{
class Model;
class Node;
}

namespace Render
{

// The MAGIC number for covertion light power from blender W to i
static const REAL light_W_to_i = 35.0;

Vector3 loadNodeTranslation(const tinygltf::Node& node);
Quaternion loadNodeRotation(const tinygltf::Node& node);
Vector3 loadNodeScale(const tinygltf::Node& node);
Matrix4 loadNodeTransformationMatrix(const tinygltf::Node& node);
ColorARGB loadColorARGB(const std::vector<double>& color);
bool loadVectorOfVec3(std::vector<Render::Vector3>& vec3, int accIndex, const tinygltf::Model& model);
bool loadVectorOfVec2(std::vector<Render::Vector2>& vec2, int accIndex, const tinygltf::Model& model);
bool loadVectorOfInt(std::vector<int>& veci, int accIndex, const tinygltf::Model& model);

//namespace Render
}
