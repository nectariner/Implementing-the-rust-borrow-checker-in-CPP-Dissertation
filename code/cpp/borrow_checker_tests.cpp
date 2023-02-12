/*
    tests for internal development only to test desired bevhaiours.
    For readers check the "test.cpp" file for relevant demonstrations
*/

//#define NDEBUG true
#include "borrow_checker.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <utility>

void increment(borrow_checked<int> value)
{
    auto mut_ref = value.get_mut_ref();
    mut_ref.set_data(*(mut_ref.get_data()) + 1);
    return;
}
namespace tests
{
// demonstrate that creating a const reference after a mutable reference results in crash
TEST(reference_counting, mut_before_const)
{
    borrow_checked<int> x(11);
    auto mut_ref = x.get_mut_ref();
    ASSERT_DEATH(auto y = x.get_const_ref(), ".*cannot create const reference when mutable reference exists.*");
}

// demonstrates that creating a mutable reference after a const reference results in crash
TEST(reference_counting, const_before_mut)
{
    borrow_checked<int> j(11);
    auto mut_ref = j.get_const_ref();

    ASSERT_DEATH(auto y = j.get_mut_ref(), ".*");
    ASSERT_EQ(1, 1);
}

// demonstrates that we can have multiple
TEST(reference_counting, many_const_ref)
{
    borrow_checked<int> j(10);
    {
        std::array<borrow_checked<int>, 4> constRefs{j.get_const_ref(), j.get_const_ref(), j.get_const_ref(),
                                                     j.get_const_ref()};
        // demonstrate reference counting
        ASSERT_EQ(j.get_const_ref_count(), 4);

        // demonstrate all references point to same data
        for (auto &reference : constRefs)
        {
            ASSERT_EQ(*(reference.get_data()), 10);
        }
    }
    // demonstrate reference counts decrease as references go out of scope (destructor called)
    ASSERT_EQ(j.get_const_ref_count(), 0);
}

TEST(reference_counting, multiple_mut_refs)
{
    borrow_checked<int> j(10);
    auto x = j.get_mut_ref();
    ASSERT_DEATH(auto y = j.get_mut_ref(), ".*");
}

TEST(value_modifications, increment)
{
    borrow_checked<int> j(10);
    {
        auto mut_ref = j.get_mut_ref();
        mut_ref.set_data(15);
    }
    auto read = j.get_const_ref();
    ASSERT_EQ(15, *read.get_data());
}

TEST(value_modifications, new_string)
{
    borrow_checked<std::string> myString(std::string{"hello"});
    auto mut_ref = myString.get_mut_ref();

    auto data = mut_ref.get_data();
    ASSERT_EQ(strcmp("hello", data->c_str()), 0);
    mut_ref.set_data(std::string{"goodbye"});
    ASSERT_EQ(strcmp("goodbye", mut_ref.get_data()->c_str()), 0);
}

// demonstrates that passing a borrow_checked by value (copying) still maintains the rules set
TEST(value_modifications, try_increment)
{
    borrow_checked<int> myNum(10);
    auto const_ref = myNum.get_const_ref();
    ASSERT_DEATH(increment(myNum.get_mut_ref()), ".*");
}

} // namespace tests

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
