
#include "util.h"
#include "material.h"

namespace Render
{
namespace GGX
{

REAL partialGeometry(REAL cosThetaN, REAL alpha)
{
    REAL cosTheta_sqrt = Util::clamp(cosThetaN * cosThetaN);
    REAL tan2 = (1 - cosTheta_sqrt) / cosTheta_sqrt;
    REAL GP = 2.0 / (1 + SQRT(1 + alpha * alpha * tan2));
    return GP;
}

REAL distribution(REAL cosThetaNH, REAL alpha)
{
    REAL alpha2 = alpha * alpha;
    REAL NH_sqr = Util::clamp(cosThetaNH * cosThetaNH);
    REAL den = NH_sqr * alpha2 + (1.0 - NH_sqr);
    return alpha2 / ( MATH_PI * den * den );
}

Vector3 fresnelSchlick(const Vector3& f0, REAL cosTheta)
{
    return f0 + (1.0 - f0) * pow(1.0 - Util::clamp(cosTheta), 5.0); //TODO можно апроксимировать через таблицу
}

Vector3 cookTorrance(Vector3 n, Vector3 l, Vector3 v, const Material& m)
{
//    n = normalize(n);
//    v = normalize(v);
//    l = normalize(l);
    Vector3 h = v + l;
    h.normalize();

    //precompute dots
    REAL NL = n & l;
    if (NL <= 0.0)
    {
        return 0.0;
    }

    REAL NV = n & v;
    if (NV <= 0.0)
    {
        return 0.0;
    }

    REAL NH = n & h;
    REAL HV = h & v;

    //calc coefficients
    REAL G = partialGeometry(NV, m.roughnessSqr()) * partialGeometry(NL, m.roughnessSqr());
    REAL D = distribution(NH, m.roughnessSqr());
    Vector3 F = fresnelSchlick(m.f0(), HV);

    //mix
    Vector3 specK = G * D* F * 0.25 / NV;

    return specK.clampMax(0.0);
}

} //namespace GGX
} //namespace Render

