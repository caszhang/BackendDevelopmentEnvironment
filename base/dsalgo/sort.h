// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef SORT_H_
#define SORT_H_
#include "head.h"
namespace internal_sort
{

// QSort begin
uint32_t Partition(void *data,
                   uint32_t low,
                   uint32_t high,
                   uint32_t each_element_size,
                   int32_t (*compare)(const void*, const void*),
                   char *pivot);

void QuickSort(void *data,
               uint32_t element_num,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*),
               char *pivot_buf);
void QSort(void *data,
           uint32_t element_num,
           uint32_t each_element_size,
           int32_t (*compare)(const void*, const void*));
// QSort end

// HeapSort begin

void HeapAdjust(void *data,
                int32_t begin,
                int32_t end,
                uint32_t each_element_size,
                int32_t (*compare)(const void*, const void*),
                char *ele_buf);

void HeapSort(void *data, uint32_t element_num,
              uint32_t each_element_size,
              int32_t (*compare)(const void*, const void*));

// HeapSort end

// MergeSort begin

void Merge(void *data, int32_t low_idx,
           int32_t mid_idx, int32_t high_idx,
           uint32_t each_element_size,
           int32_t (*compare)(const void*, const void*),
           char *ele_buf);
void DoMergeSort(void *data, int32_t low,
               int32_t high,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*),
               char *ele_buf);
void MergeSort(void *data, uint32_t element_num,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*));


// MergeSort end

}
#endif // SORT_H_
