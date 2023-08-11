#pragma once

namespace tinygltf
{
struct Node;
class Model;
}


namespace Render
{
class ILight;

namespace LightFactory
{

ILight* loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

}  //namespace Render::LightFactory

} //namespace Render
