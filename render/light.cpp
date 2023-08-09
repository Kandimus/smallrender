
#include "light.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace {
std::string nameLightExtension = "KHR_lights_punctual";

std::string nameLightDirectional = "";
std::string nameLightPoint = "point";
};

namespace Render
{

ColorRGB Light::m_globalColor(1.0);

bool Light::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{
    if (node.light < 0 || model.lights.size() <= node.light)
    {
        return false;
    }

    auto& light = model.lights[node.light];

    m_name = light.name;
    m_position = Vector3::c0;

    if (node.translation.size())
    {
        m_position.x() = node.translation[0];
        m_position.y() = node.translation[0];
        m_position.z() = -node.translation[0];
    }

    if (light.type == ::nameLightPoint)
    {
        m_type = Type::POINT;

        m_range = light.intensity;

    }
    return true;
}

} //namespace Render
