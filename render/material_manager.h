#pragma once

#include "defines.h"

#include <ostream>
#include <vector>

namespace tinygltf
{
class Model;
}

namespace Render
{

class Material;

class MaterialManager
{
public:
    MaterialManager(std::ostream& output);
    virtual ~MaterialManager();

    const Material* getMaterial(int uid) const;

    bool loadFromTinygltf(const tinygltf::Model& model);

protected:
    std::ostream& out;

    std::vector<const Material*> m_list;
};

} // namespace Render
