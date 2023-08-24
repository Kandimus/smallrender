#include "ob_box.h"
#include "ob_sphere.h"

namespace Render
{

namespace ObVolumeFactory
{

IObVolume* create(const std::vector<Vector3>& data)
{
    Vector3 minPoint = Vector3::c0;
    Vector3 maxPoint = Vector3::c0;

    for (auto& p : data)
    {
        minPoint.x() = std::min(minPoint.x(), p.x());
        minPoint.y() = std::min(minPoint.y(), p.y());
        minPoint.z() = std::min(minPoint.z(), p.z());

        maxPoint.x() = std::max(maxPoint.x(), p.x());
        maxPoint.y() = std::max(maxPoint.y(), p.y());
        maxPoint.z() = std::max(maxPoint.z(), p.z());
    }
    Vector3 size = maxPoint - minPoint;
    REAL delta = 0.2;
    REAL minDelta = 1 - delta;
    REAL maxDelta = 1 + delta;
    REAL dy = ABSR(size.x() / size.y());
    REAL dz = ABSR(size.x() / size.z());
    bool isSphere = (minDelta <= dy && dy <= maxDelta) && (minDelta <= dz && dz <= maxDelta);

    IObVolume* obv = nullptr;

    if (isSphere)
    {
        obv = new ObSphere();
        obv->create(data);
    }
    else
    {
        obv = new ObBox();
        obv->create(data);
    }

    return obv;
}

}  //namespace Render::ObVolumeFactory

} //namespace Render
