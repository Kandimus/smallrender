
#include <limits>
#include <iostream>

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

static std::vector<const Triangle*> gTriangle;

static LightAmbient gLightAmbient(Vector3(1, 1, 1));

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

    // Creating the full list of triangles
    gTriangle.clear();
    for (auto sm : gStaticMesh)
    {
        for (auto& t : sm->triangle())
        {
            gTriangle.push_back(&t);
        }
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
    Vector3 point;

    c = ColorRGB::Black;

    REAL len = triangle.intersect(ray, point);
    if (len < 0)
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

        // проверка наличие препятствия между точкой и истоником света
        bool intersected = false;
        Ray ray_to_light = light->ray(point);
        Vector3 p;

        for (auto t : gTriangle)
        {
            if (t == &triangle) continue;

            if (t->intersect(ray_to_light, p) >= MATH_EPS)
            {
                intersected = true;
                break;
            }
        }

        if (!intersected)
        {
            diffuse += light->intensity(ray, point, triangle.normal());
        }
    }

    c = Render::ColorRGB::White * gLightAmbient.ambient() + diffuse * Render::ColorRGB::White;
    c.scaleByMax();

    return len;
}

}

