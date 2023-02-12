/**
 * @file
 * @author Fred Cook <fred.cook.work@gmail.com>
 * @version 1.0
 *
 * */

#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream> //can be removed later when debugging done

/// Enum class to represent whether the referent has mutable references, immutable references, or neither (or an invalid
/// state)
enum class State
{
    unset, /// Default state, no references to referent
    mut_ref,
    const_ref,
    invalid,
};

/**
 *  Class which refers to arbitrary data and counts references of different types (mutable and immutable)
 **/
template <typename T> class borrow_checked
{
  private:
    T *data;
    int *const_ref_count;
    int *mut_ref_count;
    State *state;

    int increment_mut_ref_count()
    {
        assert(*const_ref_count == 0);
        assert(*mut_ref_count == 0);
        /*
        if (*const_ref_count != 0)
            throw;
        if (*mut_ref_count != 0)
            throw;
        */

        ++*mut_ref_count;
        *state = State::mut_ref;
        return 1;
    }

    const int increment_const_ref_count()
    {
        assert((*mut_ref_count) == 0 && "cannot create const reference when mutable reference exists");
        /*
        if (*mut_ref_count != 0)
            throw;
            */
        ++*const_ref_count;
        *state = State::const_ref;
        return *const_ref_count;
    }

    borrow_checked()
    {
    }

  public:
    // borrow_checked() : data{}, const_ref_count{new int(0)}, mut_ref_count{new int(0)}, state{unset} {}
    /// only constructor allowed
    /// @param _data describes the data that will be referred to
    borrow_checked(T _data) : data{}
    {
        state = new State(State::unset);
        data = new T(_data);
        mut_ref_count = new int(0);
        const_ref_count = new int(0);
    }

    borrow_checked get_mut_ref()
    {
        increment_mut_ref_count();
        borrow_checked new_borrow_checked;
        new_borrow_checked.data = data;
        new_borrow_checked.const_ref_count = const_ref_count;
        new_borrow_checked.mut_ref_count = mut_ref_count;
        new_borrow_checked.state = state;

        return new_borrow_checked;
    }

    borrow_checked get_const_ref()
    {
        increment_const_ref_count();
        borrow_checked new_borrow_checked;
        new_borrow_checked.data = data;
        new_borrow_checked.const_ref_count = const_ref_count;
        new_borrow_checked.mut_ref_count = mut_ref_count;
        new_borrow_checked.state = state;

        return new_borrow_checked;
    }

    void set_to_nullptr()
    {
        data = nullptr;
        return;
    }

    ~borrow_checked()
    {
        // no more references, all data can be freed
        if (*state == State::unset && *const_ref_count == 0 && *mut_ref_count == 0)
        {
            if (data != nullptr)
                free(data);
            if (const_ref_count != nullptr)
                free(const_ref_count);
            if (mut_ref_count != nullptr)
                free(mut_ref_count);
            if (state != nullptr)
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

    void set_data(T new_data)
    {
        if (get_state() == State::mut_ref)
        {
            *data = new_data;
        }
        else
        {
            // create a proper error class;
            // this is fatal, inconsistent state
            throw;
        }
    }

    const State get_state()
    {
        return *state;
    }

    const T *get_data()
    {
        return data;
    }

    const void print_state()
    {
        switch (*state)
        {
        case State::unset:
            std::cout << "state: unset\n";
            break;
        case State::const_ref:
            std::cout << "state: const_ref\n";
            break;
        case State::mut_ref:
            std::cout << "state: mut_ref\n";
            break;

        case State::invalid:
            std::cout << "state: invalid\n";
            break;
        default:
            break;
        }
    }
    const int get_const_ref_count()
    {
        return *const_ref_count;
    }
    const int get_mut_ref_count()
    {
        return *mut_ref_count;
    }
};

// template <typename T>
// bool operator==(const T lhs, const S<T> rhs) { return lhs == rhs.data; }
// template <typename T>
// bool operator!=(const T lhs, const S<T> rhs) { return lhs != rhs.data; }
//
// template <typename T>
// bool operator==(const S<T> lhs, const T rhs) { return rhs == lhs.data; }
// template <typename T>
// bool operator!=(const S<T> lhs, const T rhs) { return rhs != lhs.data; }
