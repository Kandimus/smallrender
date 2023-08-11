
#include "light.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace {
//std::string nameLightExtension = "KHR_lights_punctual";
//std::string nameLightDirectional = "";
std::string nameLightPoint = "point";
};

namespace Render
{

bool Light::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{

}

} //namespace Render
