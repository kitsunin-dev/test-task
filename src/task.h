#ifndef TASK_H
#define TASK_H

#include <array>
#include <string>

#define MINS 60

struct initial
{
    unsigned int tables;
    std::array<unsigned int, 2> open_time;
    std::array<unsigned int, 2> close_time;
    unsigned int hour_cost;
};

struct event
{
    std::array<unsigned int, 2> time;
    unsigned int id;
    std::string name;
    unsigned int table;
};

std::string int_to_str(std::array<unsigned int, 2> time);
std::array<unsigned int, 2> str_to_int(std::string time);
bool compare_time(std::array<unsigned int, 2> time1, std::array<unsigned int, 2> time2);
int spent_hours(std::array<unsigned int, 2> time1, std::array<unsigned int, 2> time2);

int process_file(std::ifstream& file);

#endif // TASK_H