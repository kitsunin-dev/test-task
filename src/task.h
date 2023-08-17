#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::array;
using std::vector;
using std::set;
using std::queue;

#define MINS 60

string int_to_str(array<unsigned int, 2> time)
{
    string output;
    if (time[0] < 10)
        output.append("0").append(std::to_string(time[0]));
    else
        output.append(std::to_string(time[0]));

    output.append(":");

    if (time[1] < 10)
        output.append("0").append(std::to_string(time[1]));
    else
        output.append(std::to_string(time[1]));

    return output;
}

array<unsigned int, 2> str_to_int(string time)
{
    return {static_cast<unsigned int>(stoi(time.substr(0, 2))), 
            static_cast<unsigned int>(stoi(time.substr(3, 2)))};
}

bool compare_time(array<unsigned int, 2> time1, array<unsigned int, 2> time2)
{
    if (time1[0] < time2[0] || (time1[0] == time2[0] && time1[0] < time2[1]))
        return false;
    else
        return true;
}

int spent_hours(array<unsigned int, 2> time1, array<unsigned int, 2> time2)
{
    return ceil((time2[0] + (float) time2[1] / MINS) - (time1[0] + (float) time1[1] / MINS));
}

struct initial
{
    unsigned int tables;
    array<unsigned int, 2> open_time;
    array<unsigned int, 2> close_time;
    unsigned int hour_cost;
};

struct event
{
    array<unsigned int, 2> time;
    unsigned int id;
    string name;
    unsigned int table;
};

#endif // TASK_H