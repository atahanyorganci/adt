#include "adt/List.h"
#include <catch2/catch.hpp>

TEST_CASE("ListTest", "[List]") {
    const size_t SIZE = 10;
    auto l = List<int>(SIZE);
    int data = 0;

    // empty list edge cases
    REQUIRE(l.is_empty());
    REQUIRE(l.len() == 0);
    REQUIRE_FALSE(l.insert(3, 3));
    REQUIRE_FALSE(l.insert(1, 3));
    REQUIRE_FALSE(l.get(1, data));

    // copy constructor, and assignment operator
    auto l2 = List<int>(l);
    auto l3 = List<int>();
    l2 = l;
    l3 = l;

    // add data
    size_t count = l.capacity() / 2;
    UNSCOPED_INFO("Adding data until list half is full.");
    for (size_t i = 0; i < count; i++)
        REQUIRE(l.insert(0, i));
    REQUIRE_FALSE(l.is_empty());
    REQUIRE(l.len() == count);
    REQUIRE_FALSE(l.get(count, data));

    // copy constructor, and assignment operator
    auto l4 = List<int>(l);
    auto l5 = List<int>();
    l4 = l;
    l5 = l;

    // resizing shouldn't leak memory
    count = l.len();
    REQUIRE_FALSE(l.resize(count - 1));
    REQUIRE(l.resize(count));
    REQUIRE_FALSE(l.insert(0, 1));
    REQUIRE(l.resize(SIZE));

    // fill rest of the list
    size_t remaining = SIZE - l.len();
    for (size_t i = 0; i < remaining; i++)
        REQUIRE(l.insert(0, i));
    REQUIRE(l.len() == l.capacity());
    REQUIRE_FALSE(l.insert(1, 2));

    // copy constructor, and assignment operator
    auto l6 = List<int>(l);
    auto l7 = List<int>();
    l6 = l;
    l7 = l;

    // Remove all items from the list
    count = l.capacity();
    for (size_t i = 0; i < count; i++)
        REQUIRE(l.remove(0));
    REQUIRE_FALSE(l.remove(1));
    REQUIRE(l.is_empty());

    // Test newly added members
    const int new_data = 10;
    REQUIRE(l.insert(0, new_data));
    REQUIRE(l.get(0, data));
    REQUIRE(data == new_data);
    REQUIRE(l.remove(0));
}
