/**
 * @file
 * @author Fred Cook <fred.cook.work@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
This file is part of "Rust Borrow Checker in C++".

"Rust Borrow Checker in C++" is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

"Rust Borrow Checker in C++" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <iostream>

/// Enum class to represent whether the referent has mutable references, immutable references, or neither (or an invalid state)
enum class State
{
    unset, /// Default state, no references to referent
    mut_ref,
    const_ref,
    invalid,
};

/**
 *  @brief Struct which refers to arbitrary data and counts references of different types (mutable and immutable)
 **/
struct S
{
private:
    int *data;
    int *const_ref_count;
    int *mut_ref_count;
    State *state;

    std::size_t increment_mut_ref_count()
    {
        assert(*const_ref_count == 0);
        assert(*mut_ref_count == 0);
        ++*mut_ref_count;
        *state = State::mut_ref;
        return 1;
    }

    std::size_t increment_const_ref_count()
    {
        assert(*mut_ref_count == 0);
        ++*const_ref_count;
        *state = State::const_ref;
        return *const_ref_count;
    }

    S() {}

public:
    // S() : data{}, const_ref_count{new int(0)}, mut_ref_count{new int(0)}, state{unset} {}
    /// only constructor allowed
    /// @param _data describes the data that will be referred to
    S(int _data) : data{}
    {
        state = new State(State::unset);
        data = new int(_data);
        mut_ref_count = new int(0);
        const_ref_count = new int(0);
    }

    S get_mut_ref()
    {
        increment_mut_ref_count();
        S new_s;
        new_s.data = data;
        new_s.const_ref_count = const_ref_count;
        new_s.mut_ref_count = mut_ref_count;
        new_s.state = state;

        return new_s;
    }

    S get_const_ref()
    {
        S new_s;
        new_s.data = data;
        new_s.const_ref_count = const_ref_count;
        new_s.mut_ref_count = mut_ref_count;
        new_s.state = state;

        increment_const_ref_count();
        return new_s;
    }

    ~S()
    {
        std::cout << "destructor\n";
        if (*state == State::unset)
        {
            free(data);
            free(const_ref_count);
            free(mut_ref_count);
            free(state);
        }
        else
        {
            if (*state == State::const_ref)
            {
                --*const_ref_count;
                if (*const_ref_count == 0)
                    *state = State::unset;
            }
            else if (*state == State::mut_ref)
            {
                --*mut_ref_count;
                if (*mut_ref_count == 0)
                    *state = State::unset;
            }
        }
    }

    void set_data(int new_data)
    {
        if (get_state() == State::mut_ref)
        {
            std::cout << "bruh\n";
            *data = new_data;
        }
        else
        {
            std::cout << "how does state !- mut_ref??\n";
            // create a proper error class;
            throw;
        }
    }

    State get_state()
    {
        return *state;
    }

    void print_state()
    {
        switch (*state)
        {
        case State::unset:
            std::cout << "unset\n";
            break;
        case State::const_ref:
            std::cout << "const_ref\n";
            break;
        case State::mut_ref:
            std::cout << "mut_ref\n";
            break;

        case State::invalid:
            std::cout << "invalid state\n";
            break;
        default:
            std::cout << "invalid state\n";
            break;
        }
    }
    std::size_t get_const_ref_count() { return *const_ref_count; }
    std::size_t get_mut_ref_count() { return *mut_ref_count; }
};