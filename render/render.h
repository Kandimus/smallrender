#pragma once

#include "camera.h"
#include "static_mesh.h"

namespace Render
{
class Light;
class ColorARGB;

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
Light* addDirectionalLight(const Vector3& direction, const Vector3& diffuse, const Vector3& specular, const Vector3& ambient);
Light* addPointLight(const Vector3& position, REAL range, const Vector3& attenuation, const Vector3& diffuse,
                     const Vector3& specular, const Vector3& ambient);
const std::vector<Light*>& lights();

//
void makeProjection(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);
void makeOrtho(REAL fFOV, REAL fAspect, REAL fNear, REAL fFar, Matrix4& m/*, bool bGPU*/);

}
