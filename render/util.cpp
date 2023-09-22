#include <stdio.h>
#include <cstdint>

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


} // namespace Util

