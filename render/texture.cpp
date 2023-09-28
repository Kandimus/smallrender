#include "texture.h"

#include "vector2.h"
//#include "helper_gltf.h"
#include "util.h"

#include "tiny_gltf.h"

namespace Render
{

Texture::Texture()
{
    char buf[12] = {0};
    sprintf(buf, "%02X-%02X-%04X", m_uid >> 24, (m_uid >> 16) & 0xFF, m_uid & 0xFFFF);
    m_strUid = buf;
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



    auto clr1 = pixel(x    , y    );
    auto clr2 = pixel(x + 1, y    );
    auto clr3 = pixel(x    , y + 1);
    auto clr4 = pixel(x + 1, y + 1);

//    return pixel(x    , y    );
//    return (clr1 * (0.0 - (1 - dx) * (1 - dy)) +
//           clr2 * (0.0 - (dx    ) * (1 - dy)) +
//           clr3 * (0.0 - (1 - dx) * (dy    )) +
//            clr4 * (0.0 - (dx    ) * (dy    ))) / 4;
    return clr1 * (1 - dx) * (1 - dy) + clr2 * (dx    ) * (1 - dy) + clr3 * (1 - dx) * (dy    ) + clr4 * (dx    ) * (dy    );
}

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

    // Samplers
    //TINYGLTF_TEXTURE_FILTER_LINEAR
    //TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR

    return true;
}

}
