#include "borrow_checker.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <utility>

// define a series of functions used purely for testing
namespace testing
{
namespace normal
{
int add(int lhs, int rhs)
{
    return lhs + rhs;
}

void increment(int &value)
{
    ++value;
}

void print(int &value)
{
    std::cout << "value = " << value << '\n';
}

int get_val(int &value)
{
    return value;
}
} // namespace normal

namespace modified
{
void increment(borrow_checked<int> &value)
{
    value.set_data(*value.get_data() + 1);
}
} // namespace modified
} // namespace testing

/*
    tests of the NORMAL test suite are supposed to demonstrate shortcomings on C++ in not catching safety issues
    tests in the MODIFIED test suite are supposed to demonstrate the changed behaviour with equivalent or similar code
   introduced as part of this project
*/

/*
    This test is supposed to demonstrate a data-race
    data-races are difficult to test, as by definition they have "non-strict" behvaiour, meaning that the results can be
   different each time so this test can either pass OR fail by design - therefore demonstrating a data race occurs
*/
TEST(NORMAL, datarace)
{
    //    borrow_checked<int> s(10);
    int x = 10;

    for (std::size_t iterations = 0; iterations < 1000; ++iterations)
    {
        std::thread t1(testing::normal::increment, std::ref(x));
        int y = testing::normal::get_val(x);
        t1.join();

        ASSERT_NE(y, x);
    }
}

TEST(NORMAL, read_after_free)
{
    int *x = new int(10);
    int valueAtX;

    std::thread t2([&]() { valueAtX = *x; });
    std::thread t1([&]() { free(x); });
    t1.join();
    t2.join();

    ASSERT_EQ(valueAtX, 10);
}

// This test demonstrated how my implementation prevents situations where a data race occurs
// e.g. multiple mutable references
TEST(MODIFIED, datarace)
{
    borrow_checked<int> x(10);
    auto y = x.get_mut_ref();

    for (std::size_t iterations = 0; iterations < 10; ++iterations)
    {
        std::thread t1(testing::modified::increment, std::ref(y));

        // impossible to create a data race like this
        ASSERT_DEATH(int j = *(x.get_const_ref().get_data()), ".*");
        t1.join();
    }
}

TEST(MODIFIED, datarace2)
{
    borrow_checked<int> x(20);

    // as mentioned before
    // impossible to make 2 mutable references to data
    auto y = x.get_mut_ref();
    ASSERT_DEATH(x.get_mut_ref().set_data(30), "");
    ASSERT_DEATH(y.get_mut_ref().set_data(40), "");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    // make sure to run in a threadsafe style for safety
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    return RUN_ALL_TESTS();
}
