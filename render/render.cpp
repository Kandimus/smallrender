
#include <filesystem>
#include <limits>
#include <iostream>

#include "render_defines.h"
#include "render.h"

#include "color_argb.h"
#include "frustum.h"
#include "helper_gltf.h"
#include "light_ambient.h"
#include "light_factory.h"
#include "material.h"
#include "static_mesh.h"
#include "triangle.h"

#include "tiny_gltf.h"

namespace Render
{
enum FileType
{
    GLTF,
    GLB,
};

static FileType gFileType = FileType::GLTF;

static unsigned char* gBuffer = nullptr;
static int gWidth = 320;
static int gHeight = 240;

static std::vector<StaticMesh*> gStaticMesh;
static std::vector<ILight*> gLight;
static std::vector<Material*> gMaterial;

static std::vector<const Triangle*> gTriangle;

static LightAmbient gLightAmbient(Vector3(1, 1, 1));

static Material gMaterialDefault(time(nullptr));

int gDebugIntX = 0;
int gDebugIntY = 0;


Camera& camera(void)
{
    return Camera::instance();
}

unsigned char* image()
{
    return gBuffer;
}

int image_width()
{
    return gWidth;
}

int image_height()
{
    return gHeight;
}


void start()
{
    camera().reset();

    gMaterialDefault.name() = "<default material>";
    gMaterialDefault.diffuse() = ColorARGB::White;
}

void init(int w, int h)
{
    if (gBuffer)
    {
        delete[] gBuffer;
    }

    gWidth = w ? w : h * camera().frustum().aspect();
    gHeight = h;
    gBuffer = new unsigned char[gWidth * gHeight * 3];

    int max_ii = gWidth * gHeight * 3 / 4;
    int max_ii_2 = gWidth * gHeight * 3 % 4;
    for (int ii = 0; ii < max_ii; ++ii)
    {
        *((int*)gBuffer + ii) = 0;
    }
    for (int ii = 0; ii < max_ii_2; ++ii)
    {
        gBuffer[max_ii + ii] = 0;
    }

    // Creating the full list of triangles
    gTriangle.clear();
    for (auto sm : gStaticMesh)
    {
        for (auto& t : sm->triangle())
        {
            gTriangle.push_back(&t);
        }
    }

    if (w)
    {
        camera().frustum().aspect() = w / REAL(h);
        camera().frustum().update();
    }
}

void finalize()
{
    delete[] gBuffer;

    gTriangle.clear();

    if (gStaticMesh.size())
    {
        for (auto sm : gStaticMesh)
        {
            delete sm;
        }
        gStaticMesh.clear();
    }

    if (gLight.size())
    {
        for (auto l : gLight)
        {
            delete l;
        }
        gLight.clear();
    }
}

StaticMesh* createStaticMesh()
{
    auto sm = new StaticMesh();

    gStaticMesh.push_back(sm);

    return sm;
}

const std::vector<StaticMesh*>& staticMeshes()
{
    return gStaticMesh;
}

void addLight(ILight* l)
{
    gLight.push_back(l);
}

const std::vector<ILight*>& lights()
{
    return gLight;
}

LightAmbient& lightAmbient()
{
    return gLightAmbient;
}

Material* createMaterial(int uid)
{
    auto mat = new Material(uid);

    gMaterial.push_back(mat);

    return mat;
}

const Material* getMaterial(int uid)
{
    for (auto mat : gMaterial)
    {
        if (mat->uid() == uid)
        {
            return mat;
        }
    }

    return &gMaterialDefault;
}

const std::vector<const Triangle*>& triangles()
{
    return gTriangle;
}

void makeProjection(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/)
{
    REAL fTheta = REAL(0.5) * fFOV;
    REAL fH = fNear/TAN(fTheta);
    REAL fW = fH/fAspect;
    REAL fQ, fQN;

    if (0 == fFar)
    {
        fQ = 1 - c_InfiniteFarPlaneAdjust;

//        if(c_ModeD3D == GetMode())
//        {
            fQN = fNear*(c_InfiniteFarPlaneAdjust - 1);
//        }
//        else
//        {
//            fQN = fNear*(c_InfiniteFarPlaneAdjust - 2);
//        }
    }
    else
    {
//        if (c_ModeD3D == GetMode())
//        {
            fQ = fFar/(fFar - fNear);
            fQN = -fQ*fNear;
//        }
//        else
//        {
//            fQ = (fFar + fNear)/(fFar - fNear);
//            fQN = -2.0f*(fFar*fNear)/(fFar - fNear);
//        }
    }

    m = Matrix4::c0;
    m.value(0, 0) = fW;
    m.value(1, 1) = fH;

//    if(bGPU && c_ModeGL != GetMode())
//    {
//        zM.Value(2, 2) = -fQ;
//        zM.Value(2, 3) = -1.0f;
//    }
//    else
//    {
        m.value(2, 2) = fQ;
        m.value(2, 3) = 1.0f;
//    }

    m.value(3, 2) = fQN;
}

void makeOrtho(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/)
{
    REAL fThetaY = REAL(0.5) * fFOV;
    REAL fSinThetaY = SIN(fThetaY);
    REAL fThetaX = fThetaY*fAspect;
    REAL fSinThetaX = SIN(fThetaX);
    REAL fW = 1.0f/(fSinThetaX*fNear);
    REAL fH = 1.0f/(fSinThetaY*fNear);
    REAL fQ;

    if(0 == fFar)
    {
        fQ = 0;
    }
    else
    {
        fQ = 1 / (fFar - fNear);
    }

    m = Matrix4::c0;
    m.value(0, 0) = fW;
    m.value(1, 1) = fH;
    m.value(2, 2) = fQ;
    m.value(3, 3) = 1.0f;

//    if(bGPU || c_ModeGL == GetMode())
//    {
//        zM.Value(2, 2) = -zM.Value(2, 2);
//    }
}



REAL calculatePoint(const Ray& ray, const Triangle& triangle, ColorRGB& c)
{
    Intersection ti;
    Vector3 t_normal = triangle.normal();

    c = ColorRGB::Black;

    REAL len = triangle.intersect(ray, ti);
    if (len < 0)
    {
        return REAL_MAXIMUM;
    }

    if (Render::gDebugIntX == 317 && Render::gDebugIntY == (480 - 337))
    {
        volatile int a = 1;
    }

#ifdef RENDER_CALCULATED_NORMALS
    t_normal = triangle.origin() * ti.w + triangle.point1() * ti.u + triangle.point2() * ti.v;
    t_normal.normalize();
#endif

    //return 1;

    // Проверка на попадание с обратной стороны треугольника
    //TODO тут нужно будет сделать зависимость от материала, или рисуем вторую сторону
    //     или делаем вид, что мы не пересекали этот треугольник. Если рисуем, то
    //     нужно обратить нормаль
    auto rn = ray.direction() & t_normal;
    if (rn > MATH_EPS)
    {
        if (triangle.material().isDoubleSided())
        {
            t_normal = -t_normal;
        }
        else
        {
            return REAL_MAXIMUM;
        }
    }

    // ILxxx - начальная интенсивность источника
    // CLxxx - цвет источника
    // IMxxx - интенсивность материала
    // CMxxx - цвет материала
    //
    // C = RSambient * ILambient + RSemmision + RSdiffuse * SUM(IL(i)diffuse * (ni & li)) + RSspecular * SUM(IL(i)specular * max(ri & vi, 0) * ((ni & li) > 0 ? 1 : 0))

    Vector3 diffuse = Vector3::c0;

    for (auto light : gLight)
    {
        if (!light->enable())
        {
            continue;
        }

        // проверка наличие препятствия между точкой и истоником света
        bool intersected = false;
        Ray ray_to_light = light->ray(ti.point);
        Intersection li;

        FOR_EACH_TRIANGLE
        {
            const Triangle* t = POINTER_TRIANGLE;
            if (t == &triangle) continue;

            if (t->intersect(ray_to_light, li) >= MATH_EPS)
            {
                intersected = true;
                break;
            }
        }
        END_FOR_EACH_TRIANGLE

        if (!intersected)
        {
            diffuse += light->intensity(ray, ti.point, t_normal); //TODO вот тут нужно решит что делать с backside
        }
    }

    ColorARGB mat_color = triangle.material().diffuse();
    c = Render::ColorRGB::White * gLightAmbient.ambient() + diffuse * triangle.material().diffuse();
    c.scaleByMax();

    return len;
}

bool loadObjectsFromGLTF(const tinygltf::Model& model, std::string& out);

bool loadScene(const std::string& filename, std::string& out)
{
    bool res = false;
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err = "";
    std::string warn = "";
    std::string ext = std::filesystem::path(filename).extension().string();

    if (ext == ".gltf")
    {
        res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        gFileType = FileType::GLTF;
    }
    else if (ext == ".glb")
    {
        res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
        gFileType = FileType::GLB;
    }
    else
    {
        out += "ERR: Unknow file extenstion!\n";
        return false;
    }

    if (!warn.empty()) {
        out += "WARN: " + warn + "\n";
    }

    if (!err.empty()) {
        out += "ERR: " + err + "\n";
    }

    if (!res)
    {
        out += "Failed to load file: '" + filename + "'\n";
        return 1;
    }
    else
    {
        out += "Loaded file: '" + filename + "'\n";
    }

    Render::start();

    if (!loadObjectsFromGLTF(model, out))
    {
        return false;
    }

    return true;
}

bool loadNodes(const tinygltf::Model& model, const std::vector<int>& nodes, const Render::Matrix4& parent_m4, std::string& out, const std::string& shift = "")
{
    for (auto& nodeId : nodes)
    {
        if (nodeId < 0)
        {
            continue;
        }

        auto& node = model.nodes[nodeId];

        out += shift;

        if (node.mesh >= 0)
        {
            auto mesh = createStaticMesh();

            if(!mesh->loadFromTinygltf(node, model))
            {
                out += "Fault load mesh '" + node.name + "'\n";
                return false;
            }

            //mesh->tranformation(parent_m4);

            out += "Load mesh '" + mesh->name() +
                       "', points: " + std::to_string(mesh->point().size()) +
                       ", indices: " + std::to_string(mesh->index().size()) +
                       ", tirangles: " + std::to_string(mesh->triangle().size()) +
                       ",  obv: " + mesh->obVolume().type() +
                       ",  min: " + mesh->obVolume().minPoint().toString() +
                       ",  max: " + mesh->obVolume().maxPoint().toString() +
                       "\n";
        }
        else if (node.camera >= 0)
        {
            if (gFileType == FileType::GLB)
            {
                camera().setDirectionInverse();
            }

            if (!Render::camera().loadFromTinygltf(node, model))
            {
                out += "Fault load camera '" + node.name + "'\n";
                return false;
            }

            if (gFileType == FileType::GLB)
            {
                camera().setDirectionInverse();
            }

            out += "Load camera '" + camera().name() +
                       "', pos: " + camera().position().toString() +
                       ", dir " + camera().direction().toString() +
                       ", up " + camera().up().toString() +
                       ", right " + camera().right().toString() +
                       "\n";
        }
        else if (node.light >= 0)
        {
            auto light = LightFactory::loadFromTinygltf(node, model);

            if (!light)
            {
                out += "Fault load light '" + node.name + "'\n";
                return false;
            }

            addLight(light);
            out += "Load light '" + light->toString() + "'\n";
        }
        else if (node.children.size())
        {
            out += "Container '" + node.name + "' open...\n";

            Matrix4 m4 = loadNodeTransformationMatrix(node);

            if (!loadNodes(model, node.children, m4, out, shift + "   "))
            {
                return false;
            }
            out += "Container '" + node.name + "' closed\n";
        }
        else
        {
            out += "Unknow element '" + node.name + "'\n";
        }
    }

    return true;
}

bool loadObjectsFromGLTF(const tinygltf::Model& model, std::string& out)
{
    int mat_id = 0;
    for (auto& material : model.materials)
    {
        auto mat = Render::createMaterial(mat_id++);

        if(!mat->loadFromTinygltf(material))
        {
            out += "Fault load material '" + material.name + "'\n";
            return false;
        }

        out += "Load material '" + mat->name() + "' [" + mat->strUid() + "]\n";
    }

    for (auto& scene : model.scenes)
    {
        if (!loadNodes(model, scene.nodes, Render::Matrix4::c1, out))
        {
            out += "Fault load scene '" + scene.name + "'\n";
            return false;
        }
    }

    return true;
}

}

