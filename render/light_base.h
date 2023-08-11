#pragma once

#include "i_light.h"

namespace Render
{

class LightBase : public ILight
{
public:
    LightBase() { m_enable = true; }
    virtual ~LightBase() = default;

    virtual const std::string& name() const override { return m_name; }
    virtual std::string& name() override { return m_name; }

    virtual bool enable() const override { return m_enable; }
    virtual bool& enable() override  { return m_enable; }

private:
    std::string m_name = "";
    bool m_enable;
};

} //namespace Render
