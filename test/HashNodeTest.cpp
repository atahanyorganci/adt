#include "common/HashNode.h"
#include <catch2/catch.hpp>

static int deleted = 0;
class TestObj {
  private:
    int number{0};

  public:
    TestObj();
    TestObj(const int number) : number(number) {}
    ~TestObj() { deleted += 1; }
};

TEST_CASE("HashNode", "[HashNode]") {
    const int NUMBER = 42;
    int number;

    auto empty = HashNode<int>();
    REQUIRE_FALSE(empty.is_occupied()); // default is empty
    REQUIRE_FALSE(empty.get(number));   // node should not return value
    REQUIRE_FALSE(empty.unset());       // try to unset empty node
    REQUIRE(empty.set(NUMBER));         // set empty node
    REQUIRE_FALSE(empty.set(NUMBER));   // try to set empty node
    REQUIRE(empty.is_occupied());       // node should be occupied

    auto occupied = HashNode<int>(NUMBER);
    REQUIRE(occupied.is_occupied());       // node should be occupied
    REQUIRE(occupied.get(number));         // node should return value
    REQUIRE(number == NUMBER);             // returned value should be equal
    REQUIRE(occupied.unset());             // unset occupied node
    REQUIRE_FALSE(occupied.unset());       // try to unset empty node
    REQUIRE_FALSE(occupied.is_occupied()); // default is empty

    auto destructor =
        HashNode<TestObj>(TestObj(4)); // first desturctor after move
    REQUIRE(deleted == 1);             // single desctructor call
    REQUIRE(destructor.is_occupied()); // node should be occupied
    REQUIRE(destructor.unset());       // unset empty node
    REQUIRE(deleted == 2);             // second desctructor call after unset

    auto equality = HashNode<int>(NUMBER);
    REQUIRE(equality == NUMBER);
    REQUIRE_FALSE(equality != NUMBER);
}
