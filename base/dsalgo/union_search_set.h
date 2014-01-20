// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef CAS_COMMON_DS_UNION_SEARCH_SET_H
#define CAS_COMMON_DS_UNION_SEARCH_SET_H
#include "head.h"
class UnionSearchSet
{
public:
    UnionSearchSet();
    ~UnionSearchSet();
public:
    bool Init(int32_t size);
    void Release();
    int32_t Search(int32_t idx);
    bool Union(int32_t idx_a, int32_t idx_b);
    int32_t *m_father;
    int32_t *m_rank; // depth of tree
    int32_t m_size;
};
#endif // CAS_COMMON_DS_UNION_SEARCH_SET_H
