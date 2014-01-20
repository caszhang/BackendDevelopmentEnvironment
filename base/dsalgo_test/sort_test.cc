// Author: zhangguoqiang01 <80176975@qq.com>
#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"
#include "dsalgo/sort.h"
int32_t IntegerCompare(const void *param_one, const void *param_two)
{
    int32_t *one = (int32_t*)param_one;
    int32_t *two = (int32_t*)param_two;

    if (*one > *two) {
        return 1;
    } else if (*one < *two) {
        return -1;
    } else {
        return 0;
    }
}

class SortTest:public testing::Test
{
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
public:
};

TEST_F(SortTest, FirstTest)
{
    int32_t array[] = {2, 3, 4, 1, -5, 0, 7};
    printf("before:\n");
    for (uint32_t i = 0; i < sizeof(array) / sizeof(int32_t); i++) {
        printf("%d\n", array[i]);
    }

    //internal_sort::QSort((void*)array, sizeof(array) / sizeof(int32_t), sizeof(int32_t), IntegerCompare);
    //internal_sort::HeapSort((void*)array, sizeof(array) / sizeof(int32_t), sizeof(int32_t), IntegerCompare);
    internal_sort::MergeSort((void*)array, sizeof(array) / sizeof(int32_t), sizeof(int32_t), IntegerCompare);
    printf("after:\n");
    for (uint32_t i = 0; i < sizeof(array) / sizeof(int32_t); i++) {
        printf("%d\n", array[i]);
    }
}

TEST_F(SortTest, RandomTest)
{
    int32_t array_size = 10;
    srand((unsigned)time(NULL));
    int32_t *array = new int32_t[array_size];
    printf("before:\n");
    for (int32_t i = 0; i < array_size; ++i) {
       array[i] = rand() % 100;
       printf("%d\n", array[i]);
    }

    //internal_sort::QSort((void*)array, sizeof(array) / sizeof(int32_t), sizeof(int32_t), IntegerCompare);
    //internal_sort::HeapSort((void*)array, array_size, sizeof(int32_t), IntegerCompare);
    internal_sort::MergeSort((void*)array, array_size, sizeof(int32_t), IntegerCompare);
    printf("after:\n");
    for (uint32_t i = 0; i < array_size; i++) {
        printf("%d\n", array[i]);
    }
    delete[] array;
}

struct Node {
    int32_t value;
    std::string content;
    Node() {
        value = 0;
        content = "";
    }
};

int32_t StructCompare(const void *param_one, const void *param_two)
{
    Node *one = (Node*)param_one;
    Node *two = (Node*)param_two;

    if (one->value > two->value) {
        return -1;
    } else if (one->value < two->value) {
        return 1;
    } else {
        return 0;
    }
}

TEST_F(SortTest, StructTest)
{
    Node *array = new Node[8];
    char buf[8];
    printf("before:\n");
    for (int i = 0; i < 8; i++) {
        array[i].value = (i * 3 + 4) % 8;
        snprintf(buf, 8, "%d", i);
        array[i].content = buf;
        printf("idx:%s, value:%d\n", array[i].content.c_str(), array[i].value);
    }
    //internal_sort::QSort((void*)array, 8, sizeof(Node), StructCompare);
    //internal_sort::HeapSort((void*)array, 8, sizeof(Node), StructCompare);
    internal_sort::MergeSort((void*)array, 8, sizeof(Node), StructCompare);

    printf("after:\n");
    for (int i = 0; i < 8; i++) {
        printf("idx:%s, value:%d\n", array[i].content.c_str(), array[i].value);
    }

    delete[] array;
}
