#ifndef STRUCTS_H
#define STRUCTS_H

#include <array>
#include <string>
#include <set>
#include <vector>
#include <queue>

#define MINS 60

// параметры из первых 3 строк файла
struct initial
{
    uint32_t tables;
    std::array<uint32_t, 2> open_time;
    std::array<uint32_t, 2> close_time;
    uint32_t hour_cost;
};

// информация о событии
struct event
{
    std::array<uint32_t, 2> time;
    uint32_t id;
    std::string name;
    uint32_t table;
};

// переменные для обработки событий и подсчета выручки
struct proc_vars
{
    std::set<std::string> clients;
    std::vector<std::string> occupied_tables;
    std::vector<std::array<uint32_t, 2>> occupation_time;
    std::vector<uint32_t> total_occ_time;
    std::vector<uint32_t> revenue;
    std::queue<std::string> cl_queue;
};

#endif // STRUCTS_H