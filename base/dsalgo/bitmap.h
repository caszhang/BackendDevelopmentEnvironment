// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef BITMAP_H_
#define BITMAP_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F

class BitMap
{
public:
    BitMap();
    ~BitMap();
public:
    bool Init(uint32_t size);
    bool Release();
public:
    bool Set(uint32_t index);
    bool Clear(uint32_t index);
    int32_t Get(uint32_t index);
    bool Reset();
private:
    uint32_t *m_buffer;
    uint32_t m_size;
};
#endif
