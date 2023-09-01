#pragma once

#include "camera.h"
#include "color_rgb.h"

namespace Render
{
class ILight;
class LightAmbient;
class StaticMesh;
class Triangle;
class Material;

static const REAL MultiplierBackgroundAmbient = 1.05;

extern int gDebugIntX;
extern int gDebugIntY;

bool loadScene(const std::string& filename, std::string& out);

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

//
Material* createMaterial(int uid);
const Material* getMaterial(int uid);

const std::vector<const Triangle*>& triangles();

//
void makeProjection(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);
void makeOrtho(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);

REAL calculatePoint(const Ray& ray, const Triangle& t, ColorRGB& c);
}
