
#include "color_rgb.h"

namespace Render
{

void ColorRGB::scaleByMax()
{
    REAL fMax = std::max(m_red, std::max(m_green, m_blue));

    if (fMax > 1.0f)
    {
        REAL invMax = 1 / fMax;

        m_red   *= invMax;
        m_green *= invMax;
        m_blue  *= invMax;
    }
}


const ColorRGB ColorRGB::Aquamarine       (0.439216, 0.858824, 0.576471);
const ColorRGB ColorRGB::Black            (0.0,      0.0,      0.0);
const ColorRGB ColorRGB::Blue             (0.0,      0.0,      1.0);
const ColorRGB ColorRGB::BlueViolet       (0.623529f, 0.372549f, 0.623529f);
const ColorRGB ColorRGB::Brown            (0.647059f, 0.164706f, 0.164706f);
const ColorRGB ColorRGB::CadetBlue        (0.372549f, 0.623529f, 0.623529f);
const ColorRGB ColorRGB::Coral            (1.0f,      0.498039f, 0.0f);
const ColorRGB ColorRGB::CornflowerBlue   (0.258824f, 0.258824f, 0.435294f);
const ColorRGB ColorRGB::Cyan             (0.0f,      1.0f,      1.0f);
const ColorRGB ColorRGB::DarkGreen        (0.184314f, 0.309804f, 0.184314f);
const ColorRGB ColorRGB::DarkOliveGreen   (0.309804f, 0.309804f, 0.184314f);
const ColorRGB ColorRGB::DarkOrhid        (0.6f,      0.195078f, 0.8f);
const ColorRGB ColorRGB::DarkSlateBlue    (0.419608f, 0.137255f, 0.556863f);
const ColorRGB ColorRGB::DarkSlateGray    (0.184314f, 0.309804f, 0.309804f);
const ColorRGB ColorRGB::DarkSlateGrey    (0.184314f, 0.309804f, 0.309804f);
const ColorRGB ColorRGB::DarkTurquiose    (0.439216f, 0.576471f, 0.858824f);
const ColorRGB ColorRGB::DimGray          (0.329412f, 0.329412f, 0.329412f);
const ColorRGB ColorRGB::DimGrey          (0.329412f, 0.329412f, 0.329412f);
const ColorRGB ColorRGB::Firebrick        (0.9f,      0.4f,      0.3f);
const ColorRGB ColorRGB::ForestGreen      (0.137255f, 0.556863f, 0.137255f);
const ColorRGB ColorRGB::Gold             (0.8f,      0.498038f, 0.196078f);
const ColorRGB ColorRGB::Goldenrod        (0.858824f, 0.858824f, 0.439126f);
const ColorRGB ColorRGB::Gray             (0.752941f, 0.752941f, 0.752941f);
const ColorRGB ColorRGB::Green            (0.0f,      1.0f,      0.0f);
const ColorRGB ColorRGB::GreenYellow      (0.576471f, 0.858824f, 0.439216f);
const ColorRGB ColorRGB::Grey             (0.752941f, 0.752941f, 0.752941f);
const ColorRGB ColorRGB::IndianRed        (0.309804f, 0.184314f, 0.184314f);
const ColorRGB ColorRGB::Khaki            (0.623529f, 0.623529f, 0.372549f);
const ColorRGB ColorRGB::LightBlue        (0.74902f,  0.847059f, 0.847059f);
const ColorRGB ColorRGB::LightGray        (0.658824f, 0.658824f, 0.658824f);
const ColorRGB ColorRGB::LightGrey        (0.658824f, 0.658824f, 0.658824f);
const ColorRGB ColorRGB::LightSteelBlue   (0.560784f, 0.560784f, 0.737255f);
const ColorRGB ColorRGB::LimeGreen        (0.196078f, 0.8f,      0.196078f);
const ColorRGB ColorRGB::Magenta          (1.0f,      0.0f,      1.0f);
const ColorRGB ColorRGB::Maroon           (0.556863f, 0.137255f, 0.419608f);
const ColorRGB ColorRGB::MediumAquamarine (0.196078f, 0.8f,      0.6f);
const ColorRGB ColorRGB::MediumBlue       (0.196078f, 0.196078f, 0.8f);
const ColorRGB ColorRGB::MediumForestGreen(0.419608f, 0.555863f, 0.137255f);
const ColorRGB ColorRGB::MediumGoldenrod  (0.917647f, 0.917647f, 0.678431f);
const ColorRGB ColorRGB::MediumOrhid      (0.576471f, 0.439216f, 0.858824f);
const ColorRGB ColorRGB::MediumSeaGreen   (0.258824f, 0.435294f, 0.258824f);
const ColorRGB ColorRGB::MediumSlateBlue  (0.498039f, 0.0f,      1.0f);
const ColorRGB ColorRGB::MediumSpringGreen(0.498039f, 1.0f,      0.0f);
const ColorRGB ColorRGB::MediumTurquoise  (0.439216f, 0.858824f, 0.858824f);
const ColorRGB ColorRGB::MediumVioletRed  (0.858824f, 0.439216f, 0.576471f);
const ColorRGB ColorRGB::MidnightBlue     (0.184314f, 0.184314f, 0.309804f);
const ColorRGB ColorRGB::Navy             (0.137255f, 0.137255f, 0.556863f);
const ColorRGB ColorRGB::NavyBlue         (0.137255f, 0.137255f, 0.556863f);
const ColorRGB ColorRGB::Orange           (0.8f,      0.196078f, 0.196078f);
const ColorRGB ColorRGB::OrangeRed        (0.0f,      0.0f,      0.498039f);
const ColorRGB ColorRGB::Orhid            (0.858824f, 0.439216f, 0.858824f);
const ColorRGB ColorRGB::PaleGreen        (0.560784f, 0.737255f, 0.560784f);
const ColorRGB ColorRGB::Pink             (0.737255f, 0.560784f, 0.560784f);
const ColorRGB ColorRGB::Plum             (0.917647f, 0.678431f, 0.917647f);
const ColorRGB ColorRGB::Red              (1.0f,      0.0f,      0.0f);
const ColorRGB ColorRGB::Salmon           (0.435294f, 0.258824f, 0.258824f);
const ColorRGB ColorRGB::SeaGreen         (0.137255f, 0.556863f, 0.419608f);
const ColorRGB ColorRGB::Sienna           (0.556863f, 0.419608f, 0.137255f);
const ColorRGB ColorRGB::SkyBlue          (0.196078f, 0.6f,      0.8f);
const ColorRGB ColorRGB::SlateBlue        (0.0f,      0.498039f, 1.0f);
const ColorRGB ColorRGB::SpringGreen      (0.0f,      1.0f,      0.498039f);
const ColorRGB ColorRGB::SteelBlue        (0.137255f, 0.419608f, 0.556863f);
const ColorRGB ColorRGB::Tan              (0.858824f, 0.576471f, 0.439216f);
const ColorRGB ColorRGB::Thistle          (0.847059f, 0.74902f,  0.847059f);
const ColorRGB ColorRGB::Turquoise        (0.678431f, 0.917647f, 0.917647f);
const ColorRGB ColorRGB::Violet           (0.309804f, 0.184314f, 0.309804f);
const ColorRGB ColorRGB::VioletRed        (0.8,      0.196078, 0.6);
const ColorRGB ColorRGB::Wheat            (0.988235, 0.988235, 0.988235);
const ColorRGB ColorRGB::White            (1.0,      1.0,      1.0);
const ColorRGB ColorRGB::Yellow           (1.0,      1.0,      0.0);

//namespace Render
}
