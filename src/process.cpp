#include "funcs.h"
#include "structs.h"
#include <iostream>
#include <fstream>

using std::cerr;
using std::cout;
using std::endl;

using std::array;
using std::queue;
using std::set;
using std::string;
using std::vector;

int process_file(std::ifstream &file)
{
    initial init;
    event ev;
    proc_vars proc;

    string open_time_str;
    string close_time_str;

    vector<string> lines;

    ev.time = {0, 0};

    string line;

    int i = 0;
    try
    {
        while (std::getline(file, line))
        {
            if (line == "")
                continue;

            if (i < 3)
            {
                if (i == 0 || i == 2)
                    if (check_number(line))
                    {
                        cerr << "Bad line format. Cannot process line " << i + 1 << ":" << endl;
                        cout << line << endl;
                        return 1;     
                    }

                if (process_init(&init, &proc, i, line, &lines))
                    return 1;

                if (i == 1)
                {
                    open_time_str = line.substr(0, 5);
                    close_time_str = line.substr(6, 5);
                }

                if (i == 1 && (check_clock(open_time_str) || check_clock(close_time_str)))
                {
                    cerr << "Bad clock format. Cannot process line 2:" << endl;
                    cout << line << endl;
                    return 1;
                }
            }
            else
            {
                if (line[2] != ':' || line[5] != ' ' || 
                    line[7] != ' ' || check_clock(line.substr(0, 5)))
                {
                    cerr << "Bad line format. Cannot process line " << i + 1 << ":" << endl;
                    cout << line << endl;
                    return 1;
                }

                lines.push_back(line);

                if (tokenize(line, &ev, init, i))
                    return 1;

                if (process_line(line, &lines, ev, init, &proc, i))
                    return 1;
            }
            ++i;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Bad format. Cannot process line " << i + 1 << ":" << endl;
        cout << line << endl;
        return 1;
    }

    for (auto it = lines.begin(); it < lines.end(); ++it)
        cout << *it << endl;

    for (auto it = proc.clients.begin(); it != proc.clients.end(); ++it)
        cout << close_time_str << " 11 " << *it << endl;

    std::cout << close_time_str << std::endl;

    show_revenue(&proc, init);

    return 0;
}