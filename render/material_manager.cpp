#include "material_manager.h"

#include "material.h"
#include "tiny_gltf.h"

namespace Render
{

MaterialManager::MaterialManager(std::ostream& output)
    : out(output)
{
    // default material
    auto material = new Material(time(nullptr));
    material->name() = "<default material>";
    material->setDiffuseColor(ColorARGB::White);
    m_list.push_back(material);
}

MaterialManager::~MaterialManager()
{
    for (auto itm : m_list)
    {
        delete itm;
    }
    m_list.clear();
}

const Material* MaterialManager::getMaterial(int uid) const
{
    for (auto mat : m_list)
    {
        if (mat->uid() == uid)
        {
            return mat;
        }
    }
    return m_list[0];
}

bool MaterialManager::loadFromTinygltf(const tinygltf::Model& model)
{
    int mat_id = 0;
    for (auto& material : model.materials)
    {
        auto mat = new Material(mat_id++);

        m_list.push_back(mat);

        if(!mat->loadFromTinygltf(material, model))
        {
            out << "MaterialManager: error: " << "Fault load material '" << material.name << "'" << std::endl;
            return false;
        }

        out << "MaterialManager: info: Load material " << mat->toString() << std::endl;
    }

    return true;
}

}
