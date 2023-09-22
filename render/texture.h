#pragma once

#include "defines.h"
#include "color_argb.h"

namespace tinygltf
{
class Model;
class Texture;
}

namespace Render
{

class Vector2;

class Texture
{
public:
    Texture();
    virtual ~Texture();

    int uid() const { return m_uid; }
    const std::string& strUid() const { return m_strUid; }

    const std::string& name() const { return m_name; }
    std::string& name() { return m_name; }

    ColorARGB color(REAL u, REAL v) const;
    ColorARGB color(const Vector2& coord) const;

    bool loadFromTinygltf(const tinygltf::Texture& texture, const tinygltf::Model& model);
    std::string toString() const
    {
        return "{name: '" + m_name + "', uid:" + m_strUid + "}";
    }

protected:
    ColorARGB pixel(int x, int y) const;

protected:
    std::string m_name;
    int m_uid;
    std::string m_strUid;

    // image
    int m_width = 0;
    int m_height = 0;
    int m_bytePerPixel = 0;
    unsigned char* m_buffer = nullptr; // TODO проверить будет ли быстрее если сделать вектор указателей на строчки

};

} // namespace Render
