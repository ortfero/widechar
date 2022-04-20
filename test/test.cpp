#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <widechar/widechar.hpp>


TEST_SUITE("widechar") {

    TEST_CASE("ascii::narrow") {
        REQUIRE_EQ(widechar::ascii::narrow(L"qwerty"), "qwerty");
    }


    TEST_CASE("ascii::wide") {
        REQUIRE_EQ(widechar::ascii::wide("qwerty"), L"qwerty");
    }


    TEST_CASE("narrow") {
        REQUIRE_EQ(widechar::narrow(L"qwerty"), "qwerty");
    }


    TEST_CASE("wide") {
        REQUIRE_EQ(widechar::wide("qwerty"), L"qwerty");
    }

}
