#include <stdio.h>
#include <cstdint>

#include "util.h"

namespace Util
{

void fastMemClear(void* dst, unsigned int count)
{
    int64_t* dst_8 = static_cast<int64_t*>(dst);
    int count_8 = count >> 3;
    int remander = count - (count_8 << 3);

    for (int64_t* cur_8 = dst_8; cur_8 < dst_8 + count_8; ++cur_8)
    {
        *cur_8 = 0;
    }
    for (int ii = count - remander; ii < count; ++ii)
    {
        *((unsigned char*)dst + ii) = 0;
    }
}

void fastMemCopy(void* dst, const void* src, unsigned int count)
{
    int64_t* dst_8 = static_cast<int64_t*>(dst);
    int count_8 = count >> 3;
    int remander = count - (count_8 << 3);

    auto cur_dst_8 = dst_8;
    auto cur_src_8 = static_cast<const int64_t*>(src);
    for (; cur_dst_8 < dst_8 + count_8; ++cur_dst_8, ++cur_src_8)
    {
        *cur_dst_8 = *cur_src_8;
    }
    for (int ii = count - remander; ii < count; ++ii)
    {
        *((unsigned char*)dst + ii) = *((unsigned char*)src + ii);
    }
}


std::string getStrUid(Uid uid)
{
    char buf[12] = {0};
    sprintf(buf, "%02X-%02X-%04X", uid >> 24, (uid >> 16) & 0xFF, uid & 0xFFFF);
    return std::string(buf);
}

} // namespace Util

