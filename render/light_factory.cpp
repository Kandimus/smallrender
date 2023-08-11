
#include "tiny_gltf.h"
#include "light_ambient.h"
//#include "light_directional.h"
//#include "light_point.h"
//#include "light_spot.h"

namespace {
//std::string nameLightDirectional = "";
std::string nameLightPoint = "point";
};

namespace Render
{

namespace LightFactory
{

ILight* loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{
    if (node.light < 0 || model.lights.size() <= node.light)
    {
        return nullptr;
    }

    auto& light = model.lights[node.light];

    std::string name = light.name;
    Vector3 position = Vector3::c0;
    Vector3 color = Vector3::c0;

    if (node.translation.size())
    {
        position.x() = node.translation[0];
        position.y() = node.translation[0];
        position.z() = -node.translation[0];
    }

    if (light.color.size() >= 3)
    {
        color.x() = light.color[0];
        color.y() = light.color[1];
        color.z() = light.color[2];
    }

    if (light.type == ::nameLightPoint)
    {
        return nullptr;
    }


    return nullptr;
}

}  //namespace Render::LightFactory

} //namespace Render
