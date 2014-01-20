// Author: zhangguoqiang01 <80176975@qq.com>
// encapsulate BitMap,support N bit-bitmap


#ifndef NBITMAP_H_
#define NBITMAP_H_
#include "bitmap.h"

class NBitMap 
{
public:
    NBitMap();
    ~NBitMap();
public:
    bool Init(uint32_t size, uint32_t bit_num);
    bool Release();
public:
    bool Set(uint32_t index, uint32_t value);
    bool Clear(uint32_t index);
    int32_t Get(uint32_t index);
    bool Reset();
private:
    BitMap *m_bitmap;
    uint32_t m_bit_num;
    uint32_t m_max_bit_value;
};
#endif
