#ifndef FUNCS_H
#define FUNCS_H

#include "structs.h"
#include <string>
#include <array>

// прототипы пользовательских функций

std::string int_to_str(std::array<unsigned int, 2>);
bool compare_time(std::array<unsigned int, 2>, std::array<unsigned int, 2>);
int spent_hours(std::array<unsigned int, 2>, std::array<unsigned int, 2>);
int check_number(std::string);
int check_clock(std::string);
int check_name(std::string);

int process_file(std::ifstream &);
int process_init(initial *, proc_vars *, int, std::string, std::vector<std::string> *);

void show_revenue(proc_vars *, initial);
int tokenize(std::string, event *, initial, int);
int process_line(std::string, std::vector<std::string> *, event, initial, proc_vars *, int);

void tests();

#endif // FUNCS_H