#include "adt/HashMap.h"
#include <catch2/catch.hpp>
#include <iostream>
#include <string>

template <int TABLE_SIZE = DEFAULT_SIZE>
class Hasher {
  public:
    static int hash(const std::string &key) {
        int hashVal = 0;
        for (size_t i = 0; i < key.length(); i++)
            hashVal = 37 * hashVal + key[i];
        hashVal %= TABLE_SIZE;
        if (hashVal < 0)
            hashVal += TABLE_SIZE;
        return hashVal;
    };
};

static const std::string keys[] = {
    "roger", "cages", "brung", "knack", "snuck", "tooth", "needy", "casts",
    "duffs", "fools", "nixed", "frost", "votes", "colds", "husks", "oozed",
    "grime", "kebob", "lavas", "lever", "pulls", "pager", "spier", "telly",
    "resay", "shoed", "mayst", "wades", "mushy", "pease", "marks", "bebug",
    "aimer", "sware", "pluck", "moats", "clubs", "ached", "bleed", "baled",
    "death", "minks", "twiny", "otter", "vivre", "trice", "eased", "snare",
    "shorn", "bases", "names", "roily", "chase", "topoi", "sedge", "quips",
    "rooms", "swung", "carpy", "glove"};
static const size_t count = sizeof(keys) / sizeof(std::string);

TEST_CASE("HashMapTest", "[HashMapTest]") {
    auto map1 = HashMap<std::string, int, Hasher<>>();
    for (size_t i = 0; i < count; i++) {
        REQUIRE(map1.insert(keys[i], static_cast<int>(i)));
    }
    for (size_t i = 0; i < count; i++) {
        int idx;
        REQUIRE(map1.get(keys[i], idx));
        REQUIRE(idx == static_cast<int>(i));
    }
    for (size_t i = 0; i < count; i++) {
        REQUIRE(map1.remove(keys[i]));
    }

    auto map2 = HashMap<std::string, int, Hasher<>>();
    for (size_t i = 0; i < count; i++) {
        REQUIRE(map2.insert(keys[i], static_cast<int>(i)));
    }
    auto map3 = map2;
    auto map4 = HashMap<std::string, int, Hasher<>>();
    map4 = map2;
}
