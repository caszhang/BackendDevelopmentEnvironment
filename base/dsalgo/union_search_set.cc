// Author: zhangguoqiang01 <80176975@qq.com>
#include "union_search_set.h"

int32_t Max(int32_t a, int32_t b)
{
    return a > b ? a : b;
}

UnionSearchSet::UnionSearchSet()
{
    m_father = NULL;
    m_rank = NULL;
    m_size = 0;
}

UnionSearchSet::~UnionSearchSet()
{}

bool UnionSearchSet::Init(int32_t size)
{
    if (size <= 0) {
        return false;
    }

    m_father = new int32_t[size];
    m_rank = new int32_t[size];

    for (int32_t i = 0; i < size; i++) {
        m_father[i] = i;
        m_rank[i] = 1;
    }

    m_size = size;
    return true;
}

int32_t UnionSearchSet::Search(int32_t idx)
{
    if (idx >= m_size) {
        return -1;
    }
    int32_t t_a = idx;
    while (m_father[idx] != idx) {
        idx = m_father[idx];
    }
    int32_t t_b = -1;
    while (m_father[t_a] != idx) {
        t_b = m_father[t_a];
        m_father[t_a] = idx;
        m_rank[t_a] = 1;
        t_a = t_b;
    }
    return idx;
}

bool UnionSearchSet::Union(int32_t idx_a, int32_t idx_b)
{
    int32_t t_a = Search(idx_a);
    if (t_a < 0) {
        return false;
    }
    int32_t t_b = Search(idx_b);
    if (t_b < 0) {
        return false;
    }
    if (t_a == t_b) {
        return true;
    }

    if (m_rank[t_a] < m_rank[t_b]) {
        m_father[t_a] = t_b;
        m_rank[t_b] = Max(m_rank[t_a] + 1, m_rank[t_b]);
    } else {
        m_father[t_b] = t_a;
        m_rank[t_a] = Max( m_rank[t_b] + 1, m_rank[t_a]);
    }
    return true;
}

void UnionSearchSet::Release()
{
    if (NULL != m_father) {
        delete[] m_father;
        m_father = NULL;
    }

    if (NULL != m_rank) {
        delete[] m_rank;
        m_rank = NULL;
    }
    return;
}

