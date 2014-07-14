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

    // enhance quick sort begin
    
    /* Byte-wise swap two items of size SIZE. */
    #define SWAP(a, b, size)						\
    do {											\
        register size_t __size = (size);			\
        register char *__a = (a), *__b = (b);		\
        do {										\
            char __tmp = *__a;						\
            *__a++ = *__b;							\
            *__b++ = __tmp;							\
        } while (--__size > 0);						\
    } while (0)

    /* Discontinue quicksort algorithm when partition gets below this size.
       This particular magic number was chosen to work best on a Sun 4/260. */
    #define MAX_THRESH 4

    /* Stack node declarations used to store unfulfilled partition obligations. */
    typedef struct {
        char *lo;
        char *hi;
    } stack_node;

    /* The next 4 #defines implement a very fast in-line stack abstraction. */
    /* The stack needs log (total_elements) entries (we could even subtract
       log(MAX_THRESH)).  Since total_elements has type size_t, we get as
       upper bound for log (total_elements):
       bits per byte (CHAR_BIT) * sizeof(size_t).  */
    #define STACK_SIZE	(CHAR_BIT * sizeof(size_t))
    #define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
    #define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
    #define	STACK_NOT_EMPTY	(stack < top)
    #define min(x, y) ((x) < (y) ? (x) : (y))



    void EQSort(void * pbase, size_t total_elems, size_t size,
            int(*cmp)(const void *,const void *));
    // enhance quick sort end
}
#endif // SORT_H_
