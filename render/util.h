#pragma once

#include <string>

namespace Util
{

typedef unsigned int Uid;

void fastMemClear(void* dst, unsigned int count);
void fastMemCopy(void* dst, const void* src, unsigned int count);

std::string getStrUid(Uid uid);

} //namespace Util

