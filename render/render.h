#pragma once

#include "camera.h"
#include "color_rgb.h"

namespace Render
{
class ILight;
class LightAmbient;
class StaticMesh;
class Triangle;

static const REAL MultiplierBackgroundAmbient = 1.05;

extern int gDebugIntX;
extern int gDebugIntY;


void init(int w, int h);
void finalize();

Camera& camera(void);

unsigned char* image();
int image_width();
int image_height();

//
StaticMesh* createStaticMesh();
const std::vector<StaticMesh*>& staticMeshes();

//
LightAmbient& lightAmbient();
void addLight(ILight* l);
const std::vector<ILight*>& lights();

const std::vector<const Triangle*>& triangles();

//
void makeProjection(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);
void makeOrtho(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);

REAL calculatePoint(const Ray& ray, const Triangle& t, ColorRGB& c);
}


#ifdef RENDER_USING_CHECK_INTERSECT_MESH
    #define FOREARCH_TRIANGLE       { auto& list = Render::staticMeshes(); for (auto mesh : list) { if (mesh->intersect(ray)) { for (auto tmp_trianle_item : mesh->triangle())
    #define END_FOREARCH_TRIANGLE   }}}
    #define POINTER_TRIANGLE        (&tmp_trianle_item)
#else
    #define FOREARCH_TRIANGLE       { for (auto tmp_trianle_item : Render::triangles())
    #define END_FOREARCH_TRIANGLE   }
    #define POINTER_TRIANGLE        (tmp_trianle_item)
#endif
