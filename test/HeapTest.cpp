#include "adt/Heap.h"
#include "sorting.h"

#include <catch2/catch.hpp>

#include <iostream>
#include <stdlib.h>
#include <time.h>

TEST_CASE("HeapTest", "[Heap]") {
    int nums[] = {2,   133, 134, 137, 10,  145, 147, 151, 152, 28,
                  160, 161, 164, 40,  41,  42,  171, 168, 172, 181,
                  54,  62,  196, 73,  209, 210, 82,  81,  88,  92,
                  220, 94,  224, 105, 235, 112, 240, 123};
    const size_t len = sizeof(nums) / sizeof(int);
    auto h = Heap<int>(len);

    // Empty heap
    REQUIRE(h.is_empty());
    REQUIRE(h.get_size() == 0);

    // Insert items in random order
    for (size_t i = 0; i < len; i++) {
        REQUIRE(h.insert(nums[i]));
    }
    REQUIRE(h.get_size() == len);

    heap_sort(nums, len);

    for (int i = len - 1; i >= 0; i--) {
        int data;
        REQUIRE(h.extract_max(data));
        REQUIRE(data == nums[i]);
    }
    REQUIRE(h.is_empty());

    // Insert items in descending order
    for (int i = len - 1; i >= 0; i--) {
        REQUIRE(h.insert(nums[i]));
    }
    REQUIRE(h.get_size() == len);

    for (int i = len - 1; i >= 0; i--) {
        int data;
        REQUIRE(h.extract_max(data));
        REQUIRE(data == nums[i]);
    }
    REQUIRE(h.is_empty());

    // Insert items in ascending order
    for (size_t i = 0; i < len; i++) {
        REQUIRE(h.insert(nums[i]));
    }
    REQUIRE(h.get_size() == len);

    for (int i = len - 1; i >= 0; i--) {
        int data;
        REQUIRE(h.extract_max(data));
        REQUIRE(data == nums[i]);
    }
    REQUIRE(h.is_empty());
}
