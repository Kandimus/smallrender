
#include <limits>

#include "render.h"
#include "light_ambient.h"
#include "static_mesh.h"
#include "triangle.h"
#include "color_rgb.h"

namespace Render
{

static unsigned char* gBuffer = nullptr;
static int gWidth = 320;
static int gHeight = 240;

static std::vector<StaticMesh*> gStaticMesh;
static std::vector<ILight*> gLight;

LightAmbient gLightAmbient(Vector3(1, 1, 1));

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

void init(int w, int h)
{
    if (gBuffer)
    {
        delete[] gBuffer;
    }

    gWidth = w;
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
}

void finalize()
{
    if (gStaticMesh.size())
    {
        for (auto sm : gStaticMesh)
        {
            delete sm;
        }
        gStaticMesh.clear();
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



REAL calculatePoint(const Ray& ray, const Triangle& t, ColorRGB& c)
{
    Vector3 p;
    Vector2 uv;

    c = ColorRGB::Black;

    //if (!t.intersect(ray, p, uv))
    if (!t.intersect(ray, p))
    {
        return 1000000000;//std::numeric_limits<REAL>::infinity();
    }

    //return 1;

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

        diffuse += light->intensity(ray, p, t.normal());
    }

    c = Render::ColorRGB::White * gLightAmbient.ambient() + diffuse * Render::ColorRGB::White;
    c.scaleByMax();

    return (ray.origin() - p).length();
}

}

