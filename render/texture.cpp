#include "texture.h"

#include "vector2.h"

#include "tiny_gltf.h"

namespace Render
{

Texture::Texture()
{
}

Texture::~Texture()
{
    if (m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

ColorARGB Texture::color(REAL u, REAL v) const
{
    return color(Vector2(u, v));
}

ColorARGB Texture::color(const Vector2& coord) const
{
    REAL pointX = coord.u() * m_width;
    REAL pointY = coord.v() * m_height;
    int x = static_cast<int>(pointX);
    int y = static_cast<int>(pointY);
    REAL dx = pointX - x;
    REAL dy = pointY - y;

    // linear filtrating
    auto clr1 = pixel(x    , y    );
    auto clr2 = pixel(x + 1, y    );
    auto clr3 = pixel(x    , y + 1);
    auto clr4 = pixel(x + 1, y + 1);

    return clr1 * (1 - dx) * (1 - dy) + clr2 * dx * (1 - dy) + clr3 * (1 - dx) * dy + clr4 * dx * dy;
}

//TODO оптимизировать
ColorARGB Texture::pixel(int x, int y) const
{
    if (m_bytePerPixel == 4)
    {
        auto pixel = m_buffer + y * m_width * 4 + x * 4;
        return ColorARGB(pixel[3] / 255.0, pixel[0] / 255.0, pixel[1] / 255.0, pixel[2] / 255.0);
    }

    if (m_bytePerPixel == 3)
    {
        auto pixel = m_buffer + y * m_width * 3 + x * 3;
        return ColorARGB(1.0, pixel[0] / 255.0, pixel[1] / 255.0, pixel[2] / 255.0);
    }

    return ColorARGB::Black;
}

bool Texture::loadFromTinygltf(const tinygltf::Texture& texture, const tinygltf::Model& model)
{
    m_name = texture.name;

    if (texture.source < 0)
    {
        return false;
    }

    auto& img = model.images[texture.source];

    if (img.pixel_type != TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
    {
        return false;
    }

    if (img.image.empty())
    {
        return false;
    }

    m_width = img.width;
    m_height = img.height;
    m_bytePerPixel = img.image.size() / (m_width * m_height);
    m_buffer = new unsigned char[img.image.size()];
    Util::fastMemCopy(m_buffer, img.image.data(), img.image.size());

    //NOTE Do I need to consider filtering (samplers)?
    //TINYGLTF_TEXTURE_FILTER_LINEAR
    //TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR

    return true;
}

}
