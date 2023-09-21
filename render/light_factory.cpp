

#include "helper_gltf.h"
//#include "light_ambient.h"
#include "light_directional.h"
#include "light_point.h"
//#include "light_spot.h"
#include "render_defines.h"

#include "tiny_gltf.h"

namespace {
std::string typePoint = "point";
std::string typeDirectional = "directional";
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
    auto position = loadNodeTranslation(node);
    auto rotation = loadNodeRotation(node);
    auto m4 = loadNodeTransformationMatrix(node);
    Vector3 color = Vector3::c1;
    REAL intensity = light.intensity;

    if (light.color.size() >= 3)
    {
        color.x() = light.color[0];
        color.y() = light.color[1];
        color.z() = light.color[2];
    }

    if (light.type == ::typePoint)
    {
        // Этот параметр в gltf задан в candela (lm/sr), переводим обратно в силу свечения (W) блендера
        // и умножаем на волшебный коэффициент
        REAL watt = intensity * 4 * MATH_PI / LumensPerWatt;
        REAL Ipoint  = watt / LightWattToIntensity;

        auto l = new LightPoint(position, color, Vector3(1, 1, 1), Ipoint, Vector3(0, 0, 1)); //TODO эту настройку вынести в файл настроек
        l->name() = name;

        return l;
    }
    else if (light.type == ::typeDirectional)
    {
        REAL Idir = intensity / LumensPerWatt;
        Matrix4 mr = rotation.toRotationMatrix();

        auto dir = (-Vector3::cZ) * mr; // Луч __ДО__ источника освещения

        dir.normalize();
        auto l = new LightDirectional(dir, color, Idir);
        l->name() = name;

        return l;
    }

    return nullptr;
}

}  //namespace Render::LightFactory

} //namespace Render
