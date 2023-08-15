
#include "tiny_gltf.h"
#include "helper_gltf.h"
#include "light_ambient.h"
//#include "light_directional.h"
#include "light_point.h"
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
    Vector3 color = Vector3::c1;
    REAL intensity = light.intensity;

    if (node.translation.size())
    {
        position.x() = node.translation[0];
        position.y() = node.translation[1];
        position.z() = -node.translation[2];
    }

    if (light.color.size() >= 3)
    {
        color.x() = light.color[0];
        color.y() = light.color[1];
        color.z() = light.color[2];
    }

    if (light.type == ::nameLightPoint)
    {
        // Этот параметр в gltf задан в candela (lm/sr), переводим обратно в силу свечения (W) блендера
        // и умножаем на волшебный коэффициент
        REAL watt = intensity * 4 * MATH_PI / 683.0;
        auto l = new LightPoint(position, color, Vector3(1, 1, 1), watt / light_W_to_i, Vector3(0, 0, 1));
        l->name() = name;
        return l;
    }


    return nullptr;
}

}  //namespace Render::LightFactory

} //namespace Render
