// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// encapsulate BitMap,support N bit-bitmap

#include "nbitmap.h"

NBitMap::NBitMap()
{
    m_bitmap = NULL;
    m_bit_num = 0;
    m_max_bit_value = 0;
}

NBitMap::~NBitMap()
{}

bool NBitMap::Init(uint32_t size, uint32_t bit_num)
{
    if (bit_num <= 0) {
        return false;
    }
    m_bit_num = bit_num;
    m_bitmap = new BitMap[bit_num];
    uint32_t temp = 1;
    for (uint32_t i = 0; i < bit_num; ++i) {
        m_bitmap[i].Init(size);
        m_max_bit_value += temp;
        temp = temp<<1;
    }
    return true;
}

bool NBitMap::Set(uint32_t index, uint32_t value) 
{
    if (value > m_max_bit_value) {
        return false;
    }
    uint32_t temp = 1;
    for (uint32_t i = 0; i < m_bit_num; ++i) {
        if (temp & value) {
            m_bitmap[i].Set(index);
        }
        else {
            m_bitmap[i].Clear(index);
        }
        temp = temp<<1;
    }
    return true;
}

bool NBitMap::Clear(uint32_t index)
{
    for (uint32_t i = 0; i < m_bit_num; ++i) {
        m_bitmap[i].Clear(index);
    }
    return true;
}

int32_t NBitMap::Get(uint32_t index)
{
    uint32_t temp = 1;
    uint32_t res = 0;
    for (uint32_t i = 0; i < m_bit_num; ++i) {
        res += m_bitmap[i].Get(index) * temp;
        temp = temp<<1;
    }
    return res;
}

bool NBitMap::Release()
{
    for (uint32_t i = 0; i < m_bit_num; ++i) {
        m_bitmap[i].Release();
    }
    return true;
}
