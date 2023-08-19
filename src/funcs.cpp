#include "funcs.h"
#include "structs.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int check_number(string line)
{
    string digits = "0123456789";
    for (int i = 0; i < line.length(); ++i)
    {
        if (digits.find(line[i]) == string::npos)
            return 1;
    }
    return 0;
}

int check_clock(string time)
{
    string digits = "0123456789";

    if (digits.find(time[0]) != string::npos &&
        digits.find(time[1]) != string::npos &&
        digits.find(time[3]) != string::npos &&
        digits.find(time[4]) != string::npos &&
        time[2] == ':')
    {
        int hours = stoi(time.substr(0, 2));
        int minutes = stoi(time.substr(3, 2));
        if (hours >= 0 && hours < 24)
        {
            if (minutes >= 0 && minutes < 60)
            {
                return 0;
            }
        }
    }
    
    return 1;
}

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

bool compare_time(array<unsigned int, 2> time1, array<unsigned int, 2> time2)
{
    if (time1[0] < time2[0] || (time1[0] == time2[0] && time1[1] < time2[1]))
        return false;
    return true;
}

int spent_hours(array<unsigned int, 2> time1, array<unsigned int, 2> time2)
{
    if (time1 == time2)
        return 0;
    if (!compare_time(time1, time2))
        return ceil((time2[0] + (float)time2[1] / MINS) - (time1[0] + (float)time1[1] / MINS));
    return -1;
}

void show_revenue(proc_vars *proc, initial init)
{
    int curr;
    for (auto it = proc->occupied_tables.begin(); it < proc->occupied_tables.end(); ++it)
        if (*it != "")
        {
            curr = it - proc->occupied_tables.begin();
            proc->revenue[curr] += spent_hours(proc->occupation_time[curr], init.close_time) * init.hour_cost;
            proc->total_occ_time[curr] += init.close_time[0] * MINS + init.close_time[1] -
                                          (proc->occupation_time[curr][0] * MINS +
                                           proc->occupation_time[curr][1]);
        }

    for (int i = 0; i < proc->revenue.size(); ++i)
        cout << i + 1 << " " << proc->revenue[i] << " "
             << int_to_str({proc->total_occ_time[i] / MINS, proc->total_occ_time[i] % MINS})
             << endl;
}

int process_init(initial *init, proc_vars *proc, int i, string line, vector<string> *lines)
{
    switch (i)
    {
    case 0:

        init->tables = stoi(line);

        proc->occupied_tables.resize(init->tables);
        proc->occupation_time.resize(init->tables);
        proc->total_occ_time.resize(init->tables);
        proc->revenue.resize(init->tables);

        for (auto it = proc->occupied_tables.begin(); it != proc->occupied_tables.end(); ++it)
            *it = "";

        for (auto it = proc->occupation_time.begin(); it != proc->occupation_time.end(); ++it)
            *it = {0, 0};

        for (auto it = proc->total_occ_time.begin(); it != proc->total_occ_time.end(); ++it)
            *it = 0;

        for (auto it = proc->revenue.begin(); it != proc->revenue.end(); ++it)
            *it = 0;

        break;
    case 1:
        if (line[2] != ':' || line[5] != ' ' || line[8] != ':')
        {
            cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
            cout << line << endl;
            return 1;
        }
        else
        {
            init->open_time[0] = stoi(line.substr(0, 2));
            init->open_time[1] = stoi(line.substr(3, 2));
            init->close_time[0] = stoi(line.substr(6, 2));
            init->close_time[1] = stoi(line.substr(9, 2));

            if (compare_time(init->open_time, init->close_time))
            {
                cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
                cout << line << endl;
                return 1;               
            }

            lines->push_back(line.substr(0, 5));
            break;
        }
    case 2:
        init->hour_cost = stoi(line);
        break;
    default:
        break;
    }

    return 0;
}

int tokenize(string line, event *ev, initial init, int i)
{
    std::stringstream ss(line);
    string token;
    int j = 0;
    ev->table = 0;
    while (ss >> token)
    {
        switch (j)
        {
        case 0:
            if (stoi(token.substr(0, 2)) < ev->time[0] ||
                (stoi(token.substr(0, 2)) == ev->time[0]) &&
                    stoi(token.substr(3, 2)) < ev->time[1])
            {
                cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
                cout << line << endl;
                return 1;
            }

            ev->time[0] = stoi(token.substr(0, 2));
            ev->time[1] = stoi(token.substr(3, 2));

            if (ev->time[0] > 23 || ev->time[1] > 59)
            {
                cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
                cout << line << endl;
                return 1;
            }
            break;
        case 1:
            ev->id = stoi(token);

            if (ev->id < 1 || ev->id > 4)
            {
                cerr << "Wrong event id. Cannot process line " << i + 1 << ":" << endl;
                cout << line << endl;
                return 1;
            }
            break;
        case 2:
            ev->name = token;
            break;
        case 3:
            ev->table = stoi(token);

            if (ev->table > init.tables)
            {
                cerr << "Wrong table. Cannot process line " << i + 1 << ":" << endl;
                cout << line << endl;
                return 1;
            }
            break;
        default:
            break;
        }
        ++j;
    }

    return 0;
}

int process_line(string line, vector<string> *lines, event ev, initial init, proc_vars *proc, int i)
{
    int curr;
    switch (ev.id)
    {
    case 1:
        if (compare_time(ev.time, init.open_time) && !proc->clients.count(ev.name))
            proc->clients.insert(ev.name);
        else if (proc->clients.count(ev.name))
            lines->push_back(int_to_str(ev.time) + " 13 YouShallNotPass");
        else if (!compare_time(ev.time, init.open_time))
            lines->push_back(int_to_str(ev.time) + " 13 NotOpenYet");
        break;
    case 2:

        if (!ev.table)
        {
            cerr << "No table entered. Cannot process line " << i + 1 << ":" << endl;
            cout << line << endl;
            return 1;
        }
        if (!proc->clients.count(ev.name))
            lines->push_back(int_to_str(ev.time) + " 13 ClientUnknown");
        else if (proc->occupied_tables[ev.table - 1] != "")
            lines->push_back(int_to_str(ev.time) + " 13 PlaceIsBusy");
        else
        {
            curr = ev.table - 1;
            proc->occupied_tables[curr] = ev.name;
            proc->occupation_time[curr][0] = ev.time[0];
            proc->occupation_time[curr][1] = ev.time[1];
        }

        if (!proc->cl_queue.empty())
            if (proc->cl_queue.front() == ev.name)
                proc->cl_queue.pop();

        break;
    case 3:
        if (std::count(proc->occupied_tables.begin(), proc->occupied_tables.end(), ""))
            lines->push_back(int_to_str(ev.time) + " 13 ICanWaitNoLonger!");
        else if (proc->cl_queue.size() == init.tables)
        {
            proc->clients.erase(ev.name);
            lines->push_back(int_to_str(ev.time) + " 11 " + ev.name);
        }
        else
            proc->cl_queue.push(ev.name);

        break;
    case 4:
        if (!proc->clients.count(ev.name))
            lines->push_back(int_to_str(ev.time) + " 13 ClientUnknown");
        else
            for (auto it = proc->occupied_tables.begin(); it < proc->occupied_tables.end(); ++it)
                if (*it == ev.name)
                {
                    curr = it - proc->occupied_tables.begin();
                    proc->revenue[curr] += spent_hours(proc->occupation_time[curr], ev.time) * init.hour_cost;
                    proc->total_occ_time[curr] += ev.time[0] * MINS + ev.time[1] -
                                                  (proc->occupation_time[curr][0] * MINS +
                                                   proc->occupation_time[curr][1]);
                    if (!proc->cl_queue.empty())
                    {
                        *it = proc->cl_queue.front();
                        proc->cl_queue.pop();
                        proc->occupation_time[curr][0] = ev.time[0];
                        proc->occupation_time[curr][1] = ev.time[1];
                        lines->push_back(int_to_str(ev.time) + " 12 " + *it + " " + std::to_string(curr + 1));
                    }
                    else
                        *it = "";

                    proc->clients.erase(ev.name);

                    break;
                }
    default:
        break;
    }

    return 0;
}