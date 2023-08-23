#pragma once

#include <vector>


namespace Render
{
class IObVolume;
class Vector3;

namespace ObVolumeFactory
{

IObVolume* create(const std::vector<Vector3>& data);

}  //namespace Render::ObVolumeFactory

} //namespace Render
