
#include <filesystem>
#include <limits>
#include <iostream>

#include "scene.h"

#include "camera.h"
#include "color_argb.h"
#include "light_ambient.h"
#include "light_factory.h"
#include "material.h"
#include "matrix4.h"
#include "helper_gltf.h"
#include "static_mesh.h"
#include "triangle.h"

#include "tiny_gltf.h"

namespace Render
{

Scene::Scene(std::ostream& output)
    : out(output)
    , m_materialManager(output)
{
    m_lightAmbient = new LightAmbient(Vector3(ColorRGB::Grey25.red(), ColorRGB::Grey25.green(),
                                              ColorRGB::Grey25.blue()));
    outInfo("Create global ambient light");
    outInfo("Create default material " + m_materialManager.getMaterial(-1)->toString());
}

Scene::~Scene()
{
    delete m_lightAmbient;
    m_lightAmbient = nullptr;

    for (auto itm : m_camera)
    {
        delete itm;
    }
    m_camera.clear();

    for (auto itm : m_staticMesh)
    {
        delete itm;
    }
    m_staticMesh.clear();

    for (auto itm : m_light)
    {
        delete itm;
    }
    m_light.clear();
}

const Camera* Scene::findCamera(const std::string& name) const
{
    if (name.empty())
    {
        return m_camera.size() ? m_camera[0] : nullptr;
    }

    for (auto itm : m_camera)
    {
        if (itm->name() == name)
        {
            return itm;
        }
    }

    return nullptr;
}

void Scene::setHeight(int h)
{
    outInfo("Set height: " + std::to_string(h));

    for (auto itm : m_camera)
    {
        itm->setViewport(h);
        itm->update();
    }
}

ColorRGB Scene::lightAmbient() const
{
    return m_lightAmbient->ambient();
}


bool Scene::load(const std::string& filename)
{
    bool res = false;
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err = "";
    std::string warn = "";
    std::string ext = std::filesystem::path(filename).extension().string();

    m_fileType = FileType::UNDEF;
    m_isInit = false;

    if (ext == ".gltf")
    {
        res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        m_fileType = FileType::GLTF;
    }
    else if (ext == ".glb")
    {
        res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
        m_fileType = FileType::GLB;
    }
    else
    {
        outError("Unknow file extenstion!");
        return false;
    }

    if (warn.size())
    {
        outWarning(warn);
    }

    if (err.size())
    {
        outError(err);
    }

    if (!res)
    {
        outError("Failed to load file: '" + filename + "'");
        return false;
    }

    outInfo("Loaded file: '" + filename + "'");

    if(!loadObjects(model))
    {
        return false;
    }

    // Creating the full list of triangles
    m_triangle.clear();
    for (auto sm : m_staticMesh)
    {
        for (auto& t : sm->triangle())
        {
            m_triangle.push_back(&t);
        }
    }

    return m_isInit = true;
}

bool Scene::loadObjects(const tinygltf::Model& model)
{
    m_materialManager.loadFromTinygltf(model);

    for (auto& scene : model.scenes)
    {
        if (!loadNodes(model, scene.nodes, Render::Matrix4::c1))
        {
            outError("Fault load scene '" + scene.name + "'");
            return false;
        }
    }

    return true;
}

bool Scene::loadNodes(const tinygltf::Model& model, const std::vector<int>& nodes, const Render::Matrix4& parent_m4, const std::string& shift)
{
    for (auto& nodeId : nodes)
    {
        if (nodeId < 0)
        {
            continue;
        }

        auto& node = model.nodes[nodeId];

        if (node.mesh >= 0)
        {
            auto mesh = new StaticMesh();

            m_staticMesh.push_back(mesh);

            if (!mesh->loadFromTinygltf(node, model, m_materialManager))
            {
                outError(shift + "Fault load mesh '" + node.name + "'");
                return false;
            }

            //mesh->tranformation(parent_m4);
            outInfo("Load mesh " + mesh->toString());
        }
        else if (node.camera >= 0)
        {
            auto camera = new Camera();

            m_camera.push_back(camera);

            if (m_fileType == FileType::GLB)
            {
                camera->setDirectionInverse();
            }

            if (!camera->loadFromTinygltf(node, model))
            {
                outError("Fault load camera '" + node.name + "'");
                return false;
            }

            if (m_fileType == FileType::GLB)
            {
                camera->setDirectionInverse();
            }

            outInfo("Load camera " + camera->toString());
        }
        else if (node.light >= 0)
        {
            auto light = LightFactory::loadFromTinygltf(node, model);

            m_light.push_back(light);

            if (!light)
            {
                outError("Fault load light '" + node.name + "'");
                return false;
            }

            outInfo("Load light " + light->toString());
        }
        else if (node.children.size())
        {
            outInfo("Container '" + node.name + "' opened...");

            Matrix4 m4 = loadNodeTransformationMatrix(node);

            if (!loadNodes(model, node.children, m4, shift + "   "))
            {
                return false;
            }
            outInfo("Container '" + node.name + "' closed");
        }
        else
        {
            outError("Unknow element '" + node.name + "'. Ignore it");
        }
    }

    return true;
}

}

