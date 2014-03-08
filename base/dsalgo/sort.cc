#include "sort.h"
namespace internal_sort
{

// QSort begin
uint32_t Partition(void *data,
                   uint32_t low,
                   uint32_t high,
                   uint32_t each_element_size,
                   int32_t (*compare)(const void*, const void*),
                   char *pivot)
{
    memcpy(pivot, data, each_element_size);
    pivot[each_element_size] = '\0';

    while (low < high) {
        while (low < high && compare((void*)((char*)data + each_element_size * high), (void*)pivot) >= 0) {
            high--;
        }
        memcpy((void*)((char*)data + low * each_element_size), (void*)((char*)data + high * each_element_size), each_element_size);
        while (low < high && compare((void*)((char*)data + low * each_element_size), (void*)pivot) <= 0) {
           low++;
        }
        memcpy((void*)((char*)data + high * each_element_size), (void*)((char*)data + low * each_element_size), each_element_size);
    }

    memcpy((void*)((char*)data + low * each_element_size), pivot, each_element_size);

    return low;
}

void QuickSort(void *data,
               uint32_t element_num,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*),
               char *pivot_buf)
{
    if (element_num <= 1) {
        return;
    }
    uint32_t pivot = Partition(data, 0, element_num - 1, each_element_size, compare, pivot_buf);
    QuickSort(data, pivot, each_element_size, compare, pivot_buf);
    QuickSort((void*)((char*)data + (pivot + 1) * each_element_size), element_num - pivot - 1, each_element_size, compare, pivot_buf);
    return;
}

void QSort(void *data,
           uint32_t element_num,
           uint32_t each_element_size,
           int32_t (*compare)(const void*, const void*))
{
    char *pivot_buf = new char[each_element_size + 1];
    QuickSort(data, element_num, each_element_size, compare, pivot_buf);
    delete[] pivot_buf;
    pivot_buf = NULL;
}
// QSort end

// HeapSort begin

void HeapAdjust(void *data,
                int32_t begin,
                int32_t end,
                uint32_t each_element_size,
                int32_t (*compare)(const void*, const void*),
                char *ele_buf)
{
    int32_t old_begin = begin;
    memcpy(ele_buf, (void*)((char*)data + each_element_size * begin), each_element_size);
    ele_buf[each_element_size] = '\0';
    for (int32_t i = begin * 2 + 1; i <= end; i = i * 2 + 1) {
        if (i < end && compare(
                (void*)((char*)data + i * each_element_size),
                (void*)((char*)data + (i + 1) * each_element_size)) < 0) {
            i++;
        }
        if (compare((void*)ele_buf, (void*)((char*)data + i * each_element_size)) >= 0) {
            break;
        }
        memcpy((void*)((char*)data + begin * each_element_size),
               (void*)((char*)data + i * each_element_size), each_element_size);
        begin = i;
    }
    if (begin != old_begin) {
        memcpy((void*)((char*)data + each_element_size * begin), ele_buf, each_element_size);
    }
    return;
}

void HeapSort(void *data, uint32_t element_num,
              uint32_t each_element_size,
              int32_t (*compare)(const void*, const void*))
{
    char *ele_buf = new char[each_element_size + 1];
    int32_t max_idx = element_num - 1;
    for (int32_t i = (max_idx - 1) / 2; i >= 0; --i) {
        HeapAdjust(data, i, max_idx, each_element_size, compare, ele_buf);
    }
    for (int32_t i = max_idx; i > 0; --i) {
        // swap data[i] with data[0]
        memcpy((void*)ele_buf, (void*)((char*)data + i * each_element_size), each_element_size);
        memcpy((void*)((char*)data + i * each_element_size), data, each_element_size);
        memcpy(data, (void*)ele_buf, each_element_size);

        HeapAdjust(data, 0, i - 1, each_element_size, compare, ele_buf);
    }
    delete[] ele_buf;
    ele_buf = NULL;
    return;
}
// HeapSort end

// MergeSort begin

void Merge(void *data, int32_t low_idx,
           int32_t mid_idx, int32_t high_idx,
           uint32_t each_element_size,
           int32_t (*compare)(const void*, const void*),
           char *ele_buf)
{
    int k = 0;
    int32_t i = low_idx;
    int32_t j = mid_idx + 1;
    while (i <= mid_idx && j <= high_idx) {
        if (compare((void*)((char*)data + i * each_element_size),
            (void*)((char*)data + j * each_element_size)) < 0) {
            memcpy((void*)(ele_buf + k * each_element_size),
                   (void*)((char*)data + i * each_element_size), each_element_size);
            ++i;
            ++k;
        } else {
            memcpy((void*)(ele_buf + k * each_element_size),
                   (void*)((char*)data + j * each_element_size), each_element_size);
            ++j;
            ++k;
        }
    }
    while (i <= mid_idx) {
         memcpy((void*)(ele_buf + k * each_element_size),
                (void*)((char*)data + i * each_element_size), each_element_size);
         ++i;
         ++k;
    }

    while (j <= high_idx) {
            memcpy((void*)(ele_buf + k * each_element_size),
                   (void*)((char*)data + j * each_element_size), each_element_size);
            ++j;
            ++k;
    }

    memcpy((void*)((char*)data + low_idx * each_element_size),
           (void*)(ele_buf), k * each_element_size);
    return;
}

void DoMergeSort(void *data, int32_t low,
               int32_t high,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*),
               char *ele_buf)
{
    if (low < high) {
        int32_t mid = (low + high) / 2;
        DoMergeSort(data, low, mid, each_element_size, compare, ele_buf);
        DoMergeSort(data, mid + 1, high, each_element_size, compare, ele_buf);
        Merge(data, low, mid, high, each_element_size, compare, ele_buf);
    }
    return;
}

void MergeSort(void *data, uint32_t element_num,
               uint32_t each_element_size,
               int32_t (*compare)(const void*, const void*))
{
    char *ele_buf = new char[element_num * each_element_size + 1];
    DoMergeSort(data, 0, element_num - 1, each_element_size, compare, ele_buf);
    delete[] ele_buf;
    ele_buf = NULL;
}

// MergeSort end

}

