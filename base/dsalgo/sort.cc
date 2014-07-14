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

    // enhance quick sort begin

    /* Order size using quicksort.  This implementation incorporates
       four optimizations discussed in Sedgewick:

       1. Non-recursive, using an explicit stack of pointer that store the
       next array partition to sort.  To save time, this maximum amount
       of space required to store an array of SIZE_MAX is allocated on the
       stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
       only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
       Pretty cheap, actually.

       2. Chose the pivot element using a median-of-three decision tree.
       This reduces the probability of selecting a bad pivot value and
       eliminates certain extraneous comparisons.

       3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
       insertion sort to order the MAX_THRESH items within each partition.
       This is a big win, since insertion sort is faster for small, mostly
       sorted array segments.

       4. The larger of the two sub-partitions is always pushed onto the
       stack first, with the algorithm then concentrating on the
       smaller partition.  This *guarantees* no more than log (total_elems)
       stack size is needed (actually O(1) in this case)!  */

    void EQSort (void * pbase, size_t total_elems, size_t size,
            int(*cmp)(const void *,const void *))
    {
        register char *base_ptr = (char *) pbase;
        const size_t max_thresh = MAX_THRESH * size;

        if (total_elems == 0) {
            /* Avoid lossage with unsigned arithmetic below.  */
            return;
        }

        if (total_elems > MAX_THRESH) {
            char *lo = base_ptr;
            char *hi = &lo[size * (total_elems - 1)];
            stack_node stack[STACK_SIZE];
            stack_node *top = stack;

            PUSH (NULL, NULL);

            while (STACK_NOT_EMPTY) {
                char *left_ptr;
                char *right_ptr;

                /* Select median value from among LO, MID, and HI. Rearrange
                   LO and HI so the three values are sorted. This lowers the
                   probability of picking a pathological pivot value and
                   skips a comparison for both the LEFT_PTR and RIGHT_PTR in
                   the while loops. */

                char *mid = lo + size * ((hi - lo) / size >> 1);

                if ((*cmp) ((void *) mid, (void *) lo) < 0) {
                    SWAP (mid, lo, size);
                }

                if ((*cmp) ((void *) hi, (void *) mid) < 0) {
                    SWAP (mid, hi, size);
                } else {
                    goto jump_over;
                }

                if ((*cmp) ((void *) mid, (void *) lo) < 0) {
                    SWAP (mid, lo, size);
                }

jump_over:;
                left_ptr  = lo + size;
                right_ptr = hi - size;

                /* Here's the famous ``collapse the walls'' section of quicksort.
                   Gotta like those tight inner loops!  They are the main reason
                   that this algorithm runs much faster than others. */
                do {
                    while ((*cmp) ((void *) left_ptr, (void *) mid) < 0) {
                        left_ptr += size;
                    }

                    while ((*cmp) ((void *) mid, (void *) right_ptr) < 0) {
                        right_ptr -= size;
                    }

                    if (left_ptr < right_ptr) {
                        SWAP (left_ptr, right_ptr, size);
                        if (mid == left_ptr) {
                            mid = right_ptr;
                        } else if (mid == right_ptr) {
                            mid = left_ptr;
                        }
                        left_ptr += size;
                        right_ptr -= size;
                    } else if (left_ptr == right_ptr) {
                        left_ptr += size;
                        right_ptr -= size;
                        break;
                    }
                } while (left_ptr <= right_ptr);

                /* Set up pointers for next iteration.  First determine whether
                   left and right partitions are below the threshold size.  If so,
                   ignore one or both.  Otherwise, push the larger partition's
                   bounds on the stack and continue sorting the smaller one. */

                if ((size_t) (right_ptr - lo) <= max_thresh) {
                    if ((size_t) (hi - left_ptr) <= max_thresh) {
                        /* Ignore both small partitions. */
                        POP (lo, hi);
                    } else {
                        /* Ignore small left partition. */
                        lo = left_ptr;
                    }
                } else if ((size_t) (hi - left_ptr) <= max_thresh) {
                    /* Ignore small right partition. */
                    hi = right_ptr;
                } else if ((right_ptr - lo) > (hi - left_ptr)) {
                    /* Push larger left partition indices. */
                    PUSH (lo, right_ptr);
                    lo = left_ptr;
                } else {
                    /* Push larger right partition indices. */
                    PUSH (left_ptr, hi);
                    hi = right_ptr;
                }
            }
        }

        /* Once the BASE_PTR array is partially sorted by quicksort the rest
           is completely sorted using insertion sort, since this is efficient
           for partitions below MAX_THRESH size. BASE_PTR points to the beginning
           of the array to sort, and END_PTR points at the very last element in
           the array (*not* one beyond it!). */


        {
            char *const end_ptr = &base_ptr[size * (total_elems - 1)];
            char *tmp_ptr = base_ptr;
            char *thresh = min(end_ptr, base_ptr + max_thresh);
            register char *run_ptr;

            /* Find smallest element in first threshold and place it at the
               array's beginning.  This is the smallest array element,
               and the operation speeds up insertion sort's inner loop. */

            for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size) {
                if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0) {
                    tmp_ptr = run_ptr;
                }
            }

            if (tmp_ptr != base_ptr) {
                SWAP (tmp_ptr, base_ptr, size);
            }

            /* Insertion sort, running from left-hand-side up to right-hand-side.  */

            run_ptr = base_ptr + size;
            while ((run_ptr += size) <= end_ptr) {
                tmp_ptr = run_ptr - size;
                while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0) {
                    tmp_ptr -= size;
                }

                tmp_ptr += size;
                if (tmp_ptr != run_ptr) {
                    char *trav;

                    trav = run_ptr + size;
                    while (--trav >= run_ptr) {
                        char c = *trav;
                        char *hi, *lo;

                        for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo) {
                            *hi = *lo;
                        }
                        *hi = c;
                    }
                }
            }
        }
    }


    // enhance quick sort end 


}

