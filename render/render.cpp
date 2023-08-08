#include "render.h"

namespace Render
{

static unsigned char* gBuffer = nullptr;
static int gWidth = 320;
static int gHeight = 240;

static std::vector<StaticMesh*> gStaticMesh;

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

    camera().reset();
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

StaticMesh* addStaticMesh()
{
    auto sm = new StaticMesh();

    gStaticMesh.push_back(sm);

    return sm;
}

const std::vector<StaticMesh*>& staticMesh()
{
    return gStaticMesh;
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

}

