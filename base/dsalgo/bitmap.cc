// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>

#include "bitmap.h"

BitMap::BitMap()
{
    m_size = 0;
    m_buffer = NULL;
}

BitMap::~BitMap()
{}

bool BitMap::Init(uint32_t size)
{
    m_size = size / BITSPERWORD + 1;
    m_buffer = new uint32_t[m_size];
    Reset();
    return true;
}

bool BitMap::Reset()
{
    memset(m_buffer, 0x0, m_size * sizeof(uint32_t));
    return true;
}

bool BitMap::Set(uint32_t index)
{
    if (index > (m_size * BITSPERWORD - 1)) {
        return false;
    }

    m_buffer[index>>SHIFT] |= (1<<(index & MASK));
    return true;
}

bool BitMap::Clear(uint32_t index)
{
    if (index > (m_size * BITSPERWORD - 1)) {
        return false;
    }
    m_buffer[index>>SHIFT] &= ~(1<<(index & MASK));
    return true;
}

int32_t BitMap::Get(uint32_t index)
{
    if (index > (m_size * BITSPERWORD - 1)) {
        return -1;
    }
    return ((m_buffer[index>>SHIFT] & (1<<(index & MASK)))>>(index & MASK));    
}

bool BitMap::Release()
{
    if (m_buffer) {
        delete[] m_buffer;
        m_buffer = NULL;
    }
    return true;
}
