#pragma once

#include "camera.h"
#include "color_rgb.h"

namespace Render
{
class ILight;
class StaticMesh;
class Triangle;

void init(int w, int h);
void finalize();

Camera& camera(void);

unsigned char* image();
int image_width();
int image_height();

//
StaticMesh* addStaticMesh();
const std::vector<StaticMesh*>& staticMeshes();

//
void setAmbient(ILight* ambient);
const std::vector<ILight*>& lights();

//
void makeProjection(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);
void makeOrtho(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);

ColorRGB calculatePoint(const Ray& ray, const Triangle& t);
}
