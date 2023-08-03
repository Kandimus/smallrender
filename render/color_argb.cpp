
#include "color_argb.h"

namespace Render
{

void ColorARGB::scaleByMax()
{
    REAL fMax = std::max(m_alpha, std::max(m_rgb.red(), std::max(m_rgb.green(), m_rgb.blue())));

    if (fMax > 1.0f)
    {
        REAL invMax = 1 / fMax;

        m_alpha *= invMax;
        m_rgb   *= invMax;
    }
}

void ColorARGB::from4444(unsigned short Color)
{
    unsigned char c0 = Color & 0xff;
    unsigned char c1 = (Color >> 8) & 0xff;

    m_alpha = ((c1 >> 4) & 0xf) / REAL(15);
    m_rgb.red() = (c1 & 0xf) / REAL(15);
    m_rgb.green() = ((c0 >> 4) & 0xf) / REAL(15);
    m_rgb.blue() = (c0 & 0xf) / REAL(15);
}

void ColorARGB::from1555(unsigned short Color)
{
    unsigned char c0 = Color & 0xff;
    unsigned char c1 = (Color >> 8) & 0xff;

    m_alpha = (c1 & 0x80) ? REAL(1) : REAL(0);
    m_rgb.red() = ((c1 >> 2) & 0x1f) / REAL(31);
    m_rgb.green() = (8 * (c1 & 0x3) + ((c0 >> 5) & 0x7)) / REAL(31);
    m_rgb.blue() = (c0 & 0x1f) / REAL(31);
}

void ColorARGB::from565(unsigned short Color)
{
    unsigned char c0 = Color & 0xff;
    unsigned char c1 = (Color >> 8) & 0xff;

    m_alpha = REAL(1);
    m_rgb.red() = ((c1 >> 3) & 0x1f) / REAL(31);
    m_rgb.green() = (8*(c1 & 0x7) + ((c0 >> 5) & 0x7)) / REAL(63);
    m_rgb.blue() = (c0 & 0x1f) / REAL(31);
}

void ColorARGB::from888(unsigned int Color)
{
    unsigned char r = (Color >> 16) & 0xff;
    unsigned char g = (Color >> 8) & 0xff;
    unsigned char b = Color & 0xff;

    m_alpha = REAL(1);
    m_rgb.red() = r / REAL(255);
    m_rgb.green() = g / REAL(255);
    m_rgb.blue() = b / REAL(255);
}

void ColorARGB::from8888(unsigned int Color)
{
    unsigned char a = (Color >> 24) & 0xff;
    unsigned char r = (Color >> 16) & 0xff;
    unsigned char g = (Color >> 8) & 0xff;
    unsigned char b = Color & 0xff;

    m_alpha = a / REAL(255);
    m_rgb.red() = r / REAL(255);
    m_rgb.green() = g / REAL(255);
    m_rgb.blue() = b / REAL(255);
}



const ColorARGB ColorARGB::Aquamarine       (0.439216f, 0.858824f, 0.576471f);
const ColorARGB ColorARGB::Black            (0.0f,      0.0f,      0.0f);
const ColorARGB ColorARGB::Blue             (0.0f,      0.0f,      1.0f);
const ColorARGB ColorARGB::BlueViolet       (0.623529f, 0.372549f, 0.623529f);
const ColorARGB ColorARGB::Brown            (0.647059f, 0.164706f, 0.164706f);
const ColorARGB ColorARGB::CadetBlue        (0.372549f, 0.623529f, 0.623529f);
const ColorARGB ColorARGB::Coral            (1.0f,      0.498039f, 0.0f);
const ColorARGB ColorARGB::CornflowerBlue   (0.258824f, 0.258824f, 0.435294f);
const ColorARGB ColorARGB::Cyan             (0.0f,      1.0f,      1.0f);
const ColorARGB ColorARGB::DarkGreen        (0.184314f, 0.309804f, 0.184314f);
const ColorARGB ColorARGB::DarkOliveGreen   (0.309804f, 0.309804f, 0.184314f);
const ColorARGB ColorARGB::DarkOrhid        (0.6f,      0.195078f, 0.8f);
const ColorARGB ColorARGB::DarkSlateBlue    (0.419608f, 0.137255f, 0.556863f);
const ColorARGB ColorARGB::DarkSlateGray    (0.184314f, 0.309804f, 0.309804f);
const ColorARGB ColorARGB::DarkSlateGrey    (0.184314f, 0.309804f, 0.309804f);
const ColorARGB ColorARGB::DarkTurquiose    (0.439216f, 0.576471f, 0.858824f);
const ColorARGB ColorARGB::DimGray          (0.329412f, 0.329412f, 0.329412f);
const ColorARGB ColorARGB::DimGrey          (0.329412f, 0.329412f, 0.329412f);
const ColorARGB ColorARGB::Firebrick        (0.9f,      0.4f,      0.3f);
const ColorARGB ColorARGB::ForestGreen      (0.137255f, 0.556863f, 0.137255f);
const ColorARGB ColorARGB::Gold             (0.8f,      0.498038f, 0.196078f);
const ColorARGB ColorARGB::Goldenrod        (0.858824f, 0.858824f, 0.439126f);
const ColorARGB ColorARGB::Gray             (0.752941f, 0.752941f, 0.752941f);
const ColorARGB ColorARGB::Green            (0.0f,      1.0f,      0.0f);
const ColorARGB ColorARGB::GreenYellow      (0.576471f, 0.858824f, 0.439216f);
const ColorARGB ColorARGB::Grey             (0.752941f, 0.752941f, 0.752941f);
const ColorARGB ColorARGB::IndianRed        (0.309804f, 0.184314f, 0.184314f);
const ColorARGB ColorARGB::Khaki            (0.623529f, 0.623529f, 0.372549f);
const ColorARGB ColorARGB::LightBlue        (0.74902f,  0.847059f, 0.847059f);
const ColorARGB ColorARGB::LightGray        (0.658824f, 0.658824f, 0.658824f);
const ColorARGB ColorARGB::LightGrey        (0.658824f, 0.658824f, 0.658824f);
const ColorARGB ColorARGB::LightSteelBlue   (0.560784f, 0.560784f, 0.737255f);
const ColorARGB ColorARGB::LimeGreen        (0.196078f, 0.8f,      0.196078f);
const ColorARGB ColorARGB::Magenta          (1.0f,      0.0f,      1.0f);
const ColorARGB ColorARGB::Maroon           (0.556863f, 0.137255f, 0.419608f);
const ColorARGB ColorARGB::MediumAquamarine (0.196078f, 0.8f,      0.6f);
const ColorARGB ColorARGB::MediumBlue       (0.196078f, 0.196078f, 0.8f);
const ColorARGB ColorARGB::MediumForestGreen(0.419608f, 0.555863f, 0.137255f);
const ColorARGB ColorARGB::MediumGoldenrod  (0.917647f, 0.917647f, 0.678431f);
const ColorARGB ColorARGB::MediumOrhid      (0.576471f, 0.439216f, 0.858824f);
const ColorARGB ColorARGB::MediumSeaGreen   (0.258824f, 0.435294f, 0.258824f);
const ColorARGB ColorARGB::MediumSlateBlue  (0.498039f, 0.0f,      1.0f);
const ColorARGB ColorARGB::MediumSpringGreen(0.498039f, 1.0f,      0.0f);
const ColorARGB ColorARGB::MediumTurquoise  (0.439216f, 0.858824f, 0.858824f);
const ColorARGB ColorARGB::MediumVioletRed  (0.858824f, 0.439216f, 0.576471f);
const ColorARGB ColorARGB::MidnightBlue     (0.184314f, 0.184314f, 0.309804f);
const ColorARGB ColorARGB::Navy             (0.137255f, 0.137255f, 0.556863f);
const ColorARGB ColorARGB::NavyBlue         (0.137255f, 0.137255f, 0.556863f);
const ColorARGB ColorARGB::Orange           (0.8f,      0.196078f, 0.196078f);
const ColorARGB ColorARGB::OrangeRed        (0.0f,      0.0f,      0.498039f);
const ColorARGB ColorARGB::Orhid            (0.858824f, 0.439216f, 0.858824f);
const ColorARGB ColorARGB::PaleGreen        (0.560784f, 0.737255f, 0.560784f);
const ColorARGB ColorARGB::Pink             (0.737255f, 0.560784f, 0.560784f);
const ColorARGB ColorARGB::Plum             (0.917647f, 0.678431f, 0.917647f);
const ColorARGB ColorARGB::Red              (1.0f,      0.0f,      0.0f);
const ColorARGB ColorARGB::Salmon           (0.435294f, 0.258824f, 0.258824f);
const ColorARGB ColorARGB::SeaGreen         (0.137255f, 0.556863f, 0.419608f);
const ColorARGB ColorARGB::Sienna           (0.556863f, 0.419608f, 0.137255f);
const ColorARGB ColorARGB::SkyBlue          (0.196078f, 0.6f,      0.8f);
const ColorARGB ColorARGB::SlateBlue        (0.0f,      0.498039f, 1.0f);
const ColorARGB ColorARGB::SpringGreen      (0.0f,      1.0f,      0.498039f);
const ColorARGB ColorARGB::SteelBlue        (0.137255f, 0.419608f, 0.556863f);
const ColorARGB ColorARGB::Tan              (0.858824f, 0.576471f, 0.439216f);
const ColorARGB ColorARGB::Thistle          (0.847059f, 0.74902f,  0.847059f);
const ColorARGB ColorARGB::Turquoise        (0.678431f, 0.917647f, 0.917647f);
const ColorARGB ColorARGB::Violet           (0.309804f, 0.184314f, 0.309804f);
const ColorARGB ColorARGB::VioletRed        (0.8f,      0.196078f, 0.6f);
const ColorARGB ColorARGB::Wheat            (0.988235f, 0.988235f, 0.988235f);
const ColorARGB ColorARGB::White            (1.0f,      1.0f,      1.0f);
const ColorARGB ColorARGB::Yellow           (1.0f,      1.0f,      0.0f);

//namespace Render
}
