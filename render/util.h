#pragma once

#include <string>
#include "defines.h"

namespace Util
{

typedef unsigned int Uid;

void fastMemClear(void* dst, unsigned int count);
void fastMemCopy(void* dst, const void* src, unsigned int count);

std::string getStrUid(Uid uid);

inline REAL clamp(REAL value) { return std::min(std::max(value, REAL(0)), REAL(1)); }

} //namespace Util

