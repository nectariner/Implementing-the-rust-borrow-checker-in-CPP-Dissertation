#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>
#include <string_view>

void log(const std::source_location location = std::source_location::current())
{
    std::cout << "file: "
              << location.file_name() << "("
              << location.line() << ":"
              << location.column() << ") `"
              << location.function_name() << '\n';
}
class S
{
private:
    std::size_t mut_ref_count = 0;
    std::size_t const_ref_count = 0;

public:
    std::size_t get_mut_ref_count() { return this->mut_ref_count; }
    std::size_t get_const_ref_count() { return this->mut_ref_count; }

    void inc_mut_ref_count()
    {
        assert(get_mut_ref_count() == 0);
        assert(get_const_ref_count() == 0);

        ++mut_ref_count;
    }
    void inc_const_ref_count()
    {
        assert(get_mut_ref_count() == 0);

        ++const_ref_count;
    }

    S &make_mut_ref()
    {
        inc_mut_ref_count();
        return *this;
    }
};

void test(const S &x)
{
    return;
}

int main()
{
    std::cout << "hello\n";
    S s;
    auto &x = s.make_mut_ref();
    std::cout << s.get_mut_ref_count();
    auto &z = s.make_mut_ref();

    return 0;
}