#pragma once

namespace Util
{

void fastMemClear(void* dst, unsigned int count);
void fastMemCopy(void* dst, const void* src, unsigned int count);

} //namespace Util

