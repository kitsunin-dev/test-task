#include "funcs.h"

#include <cassert>

// юнит-тесты для некоторых функций
void tests()
{
    assert(check_number("2") == 0);
    assert(check_number("672") == 0);
    assert(check_number("-23") == 1);
    assert(check_number("64e") == 1);
    assert(check_number("3.14") == 1);

    assert(check_clock("12:00") == 0);
    assert(check_clock("00:00") == 0);
    assert(check_clock("9:00") == 1);
    assert(check_clock("34:20") == 1);
    assert(check_clock("12345") == 1);

    assert(int_to_str({2, 3}) == "02:03");
    assert(int_to_str({0, 0}) == "00:00");
    assert(int_to_str({16, 35}) == "16:35");
    assert(int_to_str({62, 11}) == "62:11");
    assert(int_to_str({90, 7}) == "90:07");

    assert(compare_time({2, 3}, {3, 4}) == false);
    assert(compare_time({5, 3}, {5, 4}) == false);
    assert(compare_time({14, 7}, {12, 7}) == true);
    assert(compare_time({1, 1}, {1, 1}) == true);
    assert(compare_time({0, 0}, {23, 59}) == false);

    assert(spent_hours({9, 55}, {13, 1}) == 4);
    assert(spent_hours({9, 55}, {9, 56}) == 1);
    assert(spent_hours({13, 7}, {16, 6}) == 3);
    assert(spent_hours({1, 5}, {1, 5}) == 0);
    assert(spent_hours({13, 7}, {12, 8}) == -1);
}