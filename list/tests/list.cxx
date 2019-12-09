#include "../src/list.hpp"

#include <catch.hpp>

SCENARIO("a list is created")
{
  GIVEN("an empty const list")
  {
    WHEN("the list is constructed trivially")
    {
      const List<int> list{};
      THEN("the list is empty") { REQUIRE(list.empty() == true); }
      AND_THEN("the size of the list is 0") { REQUIRE(list.size() == 0); }
    }
    AND_WHEN("the list is constructed from a copy a list")
    {
      const List<int> copy{};
      const List<int> list{copy};
      THEN("the list is empty") { REQUIRE(list.empty() == true); }
      AND_THEN("the size of the list is 0") { REQUIRE(list.size() == 0); }
    }
    AND_WHEN("the list is constructed from a move of a list")
    {
      const List<int> list{List<int>{}};
      THEN("the list is empty") { REQUIRE(list.empty() == true); }
      AND_THEN("the size of the list is 0") { REQUIRE(list.size() == 0); }
    }
    AND_WHEN("the list is constructed from an initializer_list")
    {
      const List<int> list{};
      THEN("the list is empty") { REQUIRE(list.empty() == true); }
      AND_THEN("the size of the list is 0") { REQUIRE(list.size() == 0); }
    }
  }
  GIVEN("a non-empty const list")
  {
    WHEN("the list is constructed from an initializer_list")
    {
      const List<int> list{1, 2, 3};
      THEN("the list is size 3") { REQUIRE(list.size() == 3); }
      AND_THEN("the list is not empty") { REQUIRE(list.empty() == false); }
    }
    AND_WHEN("the list is constructed from a copy of a list")
    {
      const List<int> copy{1, 2, 3};
      const List<int> list{copy};
      THEN("the list is size 3") { REQUIRE(list.size() == 3); }
      AND_THEN("the list is not empty") { REQUIRE(list.empty() == false); }
    }
    AND_WHEN("the list is constructed from a move of a list")
    {
      const List<int> list{List<int>{1, 2, 3}};
      THEN("the list is size 3") { REQUIRE(list.size() == 3); }
      AND_THEN("the list is not empty") { REQUIRE(list.empty() == false); }
    }
  }
}

SCENARIO("A list is modified")
{
  GIVEN("an empty const list")
  {
    const List<int> list{};
    const auto      listSize = list.size();
    WHEN("push_front")
    {
      const List<int> otherList = list.push_front(1);
      THEN("the new list has a different size")
      {
        REQUIRE(otherList.size() != list.size());
      }
      AND_THEN("the old list is unchanged")
      {
        REQUIRE(list.size() == listSize);
      }
    }
  }
  AND_GIVEN("a non-empty const list")
  {
    const List<int> list{1, 2, 3};
    const auto      listSize = list.size();
    WHEN("push_front")
    {
      const List<int> otherList = list.push_front(0);
      THEN("the new list has a different size")
      {
        REQUIRE(otherList.size() != list.size());
      }
      AND_THEN("the old list is unchanged")
      {
        REQUIRE(list.size() == listSize);
      }
    }
    AND_WHEN("pop_front")
    {
      const List<int> otherList = list.pop_front();
      THEN("the new list has a different size")
      {
        REQUIRE(otherList.size() != list.size());
      }
      AND_THEN("the old list is unchanged")
      {
        REQUIRE(list.size() == listSize);
      }
    }
  }
}

SCENARIO("A list is iterated")
{
  GIVEN("an empty const list")
  {
    const List<int> list{};
    WHEN("iterated")
    {
      auto iter = list.begin();
      THEN("the iterator is invalid") { REQUIRE(!iter.valid()); }
      AND_THEN("incrementing the iterator throws an error")
      {
        REQUIRE_THROWS_AS([&] { ++iter; }(), std::out_of_range);
        REQUIRE_THROWS_AS([&] { iter += 1; }(), std::out_of_range);
      }
      AND_THEN("trying to dereference the iterator throws an error") {}
    }
  }
  GIVEN("a non-empty const list")
  {
    const List<int> list{1, 2, 3};
    WHEN("iterated with an iterator")
    {
      auto iter = list.begin();
      THEN("the iterator is valid") { REQUIRE(iter.valid()); }
      AND_THEN("the iterator's value is the first value")
      {
        REQUIRE(**iter == 1);
      }
      AND_THEN("incrementing the iterator throws no errors")
      {
        REQUIRE_NOTHROW([&] { iter++; }());
        REQUIRE_NOTHROW([&] { iter += 1; }());
      }
      AND_THEN("going out of range produces an exception")
      {
        REQUIRE_THROWS_AS([&] { iter += 4; }(), std::out_of_range);
      }
    }
    AND_WHEN("iterated with [] operator")
    {
      THEN("we can see all the values of the list")
      {
        REQUIRE_NOTHROW([&] {
          REQUIRE(list[0] == 1);
          REQUIRE(list[1] == 2);
          REQUIRE(list[2] == 3);
        }());
      }
      AND_THEN("going out of range produces an exception")
      {
        REQUIRE_THROWS_AS(
            [&] {
              auto i = list[3];
              REQUIRE(i == 4);
            }(),
            std::out_of_range);
      }
    }
  }
}
