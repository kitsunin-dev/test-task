#ifndef STRUCTS_H
#define STRUCTS_H

#include <array>
#include <string>
#include <set>
#include <vector>
#include <queue>

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

struct proc_vars
{
    std::set<std::string> clients;
    std::vector<std::string> occupied_tables;
    std::vector<std::array<unsigned int, 2>> occupation_time;
    std::vector<unsigned int> total_occ_time;
    std::vector<unsigned int> revenue;
    std::queue<std::string> cl_queue;
};

#endif // STRUCTS_H