#include "task.h"

int main(int argc, char** argv)
{
    initial init;
    event ev;

    string open_time_str;
    string close_time_str;

    set<string> clients;
    vector<string> occupied_tables;
    vector<array<unsigned int, 2>> occupation_time;
    vector<unsigned int> total_occ_time;
    vector<unsigned int> revenue;
    queue<string> cl_queue;

    vector<string> lines;

    string line;
    std::ifstream file;

    ev.time = {0, 0};

    file.open(argv[1], std::ios::in);

    if (file.is_open())
    {
        int i = 0;
        try
        {
            while (std::getline(file, line))
            {
                if (line == "")
                    continue;

                switch (i)
                {
                    case 0:

                        init.tables = stoi(line);

                        occupied_tables.resize(init.tables);
                        occupation_time.resize(init.tables);
                        total_occ_time.resize(init.tables);
                        revenue.resize(init.tables);
                        
                        for (auto it = occupied_tables.begin(); it != occupied_tables.end(); ++it)
                            *it = "";

                        for (auto it = occupation_time.begin(); it != occupation_time.end(); ++it)
                            *it = {0, 0};

                        for (auto it = total_occ_time.begin(); it != total_occ_time.end(); ++it)
                            *it = 0;

                        for (auto it = revenue.begin(); it != revenue.end(); ++it)
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
                            init.open_time[0] = stoi(line.substr(0, 2));
                            init.open_time[1] = stoi(line.substr(3, 2));
                            init.close_time[0] = stoi(line.substr(6, 2));
                            init.close_time[1] = stoi(line.substr(9, 2));

                            open_time_str = line.substr(0, 5);
                            close_time_str = line.substr(6, 5);

                            lines.push_back(open_time_str);
                            break;
                        }
                    case 2:
                        init.hour_cost = stoi(line);
                        break;
                    default:

                        if (line[2] != ':' || line[5] != ' ' || line[7] != ' ')
                        {
                            cerr << "Bad line format. Cannot process line " << i + 1 << ":" << endl;
                            cout << line << endl;
                            return 1;  
                        }

                        lines.push_back(line);

                        std::stringstream ss(line);
                        string token;
                        int j = 0;
                        ev.table = 0;
                        while (ss >> token)
                        {
                            switch (j)
                            {
                                case 0:
                                    if (stoi(token.substr(0, 2)) < ev.time[0] || 
                                       (stoi(token.substr(0, 2)) == ev.time[0]) && 
                                        stoi(token.substr(3, 2)) < ev.time[1])
                                    {
                                        cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
                                        cout << line << endl;
                                        return 1;        
                                    } 
                                    
                                    ev.time[0] = stoi(token.substr(0, 2));
                                    ev.time[1] = stoi(token.substr(3, 2));

                                    if (ev.time[0] > 23 || ev.time[1] > 59)
                                    {
                                        cerr << "Bad clock format. Cannot process line " << i + 1 << ":" << endl;
                                        cout << line << endl;
                                        return 1;        
                                    }
                                    break;
                                case 1:
                                    ev.id = stoi(token);

                                    if (ev.id < 1 || ev.id > 4)
                                    {
                                        cerr << "Wrong event id. Cannot process line " << i + 1 << ":" << endl;
                                        cout << line << endl;
                                        return 1;        
                                    }
                                    break;
                                case 2:
                                    ev.name = token;
                                    break;
                                case 3:
                                    ev.table = stoi(token);

                                    if (ev.table > init.tables)
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

                        switch (ev.id)
                        {
                            case 1:
                                if (compare_time(ev.time, init.open_time) && !clients.count(ev.name))
                                    clients.insert(ev.name);
                                else if (clients.count(ev.name))
                                    lines.push_back(int_to_str(ev.time) + " 13 YouShallNotPass");
                                else if (!compare_time(ev.time, init.open_time))
                                    lines.push_back(int_to_str(ev.time) + " 13 NotOpenYet");
                                break;
                            case 2:

                                if (!ev.table)
                                {
                                    cerr << "No table entered. Cannot process line " << i + 1 << ":" << endl;
                                    cout << line << endl;
                                    return 1;  
                                }
                                if (!clients.count(ev.name))
                                    lines.push_back(int_to_str(ev.time) + " 13 ClientUnknown");
                                else if (occupied_tables[ev.table - 1] != "")
                                    lines.push_back(int_to_str(ev.time) + " 13 PlaceIsBusy");
                                else
                                {
                                    occupied_tables[ev.table - 1] = ev.name;
                                    occupation_time[ev.table - 1][0] = ev.time[0];
                                    occupation_time[ev.table - 1][1] = ev.time[1];
                                }

                                if (!cl_queue.empty())
                                    if (cl_queue.front() == ev.name)
                                        cl_queue.pop();

                                break;
                            case 3:
                                if(std::count(occupied_tables.begin(), occupied_tables.end(), ""))
                                    lines.push_back(int_to_str(ev.time) + " 13 ICanWaitNoLonger!");
                                else if (cl_queue.size() == init.tables)
                                {
                                    clients.erase(ev.name);
                                    lines.push_back(int_to_str(ev.time) + " 11 " + ev.name);
                                }
                                else
                                    cl_queue.push(ev.name);

                                break;
                            case 4:
                                if (!clients.count(ev.name))
                                    lines.push_back(int_to_str(ev.time) + " 13 ClientUnknown");
                                else
                                    for (auto it = occupied_tables.begin(); it < occupied_tables.end(); ++it)
                                        if (*it == ev.name)
                                        {
                                            revenue[it - occupied_tables.begin()] += spent_hours(occupation_time[it - occupied_tables.begin()], ev.time) * init.hour_cost;
                                            total_occ_time[it - occupied_tables.begin()] += ev.time[0] * MINS + ev.time[1] - 
                                                                                            (occupation_time[it - occupied_tables.begin()][0] * MINS + 
                                                                                            occupation_time[it - occupied_tables.begin()][1]);
                                            if (!cl_queue.empty())
                                            {
                                                *it = cl_queue.front();
                                                cl_queue.pop();
                                                occupation_time[it - occupied_tables.begin()][0] = ev.time[0];
                                                occupation_time[it - occupied_tables.begin()][1] = ev.time[1];
                                                lines.push_back(int_to_str(ev.time) + " 12 " + *it + " " + std::to_string(it - occupied_tables.begin() + 1));
                                            }
                                            else
                                                *it = "";

                                            clients.erase(ev.name);
                                            
                                            break;
                                        }

                            default:
                                break;
                        }

                    break;
                }
                ++i;
            }
        }
        catch(const std::exception& e)
        {
            cerr << "Bad format. Cannot process line " << i + 1 << ":" << endl;
            cout << line << endl;
            return 1;
        }   
    }
    else
    {
        std::cerr << "Unable to open the text file." << endl;
        return 1;
    }
    
    file.close();

    for (auto it = lines.begin(); it < lines.end(); ++it)
        cout << *it << endl;

    for (auto it = occupied_tables.begin(); it < occupied_tables.end(); ++it)
        if (*it != "")
        {
            revenue[it - occupied_tables.begin()] += spent_hours(occupation_time[it - occupied_tables.begin()], init.close_time) * init.hour_cost;
            total_occ_time[it - occupied_tables.begin()] += init.close_time[0] * MINS + init.close_time[1] - 
                                                            (occupation_time[it - occupied_tables.begin()][0] * MINS + 
                                                            occupation_time[it - occupied_tables.begin()][1]);
        }
    
    for (auto it = clients.begin(); it != clients.end(); ++it)
        cout << close_time_str << " 11 " << *it << endl;

    std::cout << close_time_str << std::endl;

    for (int i = 0; i < revenue.size(); ++i)
        cout << i + 1 << " " << revenue[i] << " " << int_to_str({total_occ_time[i] / MINS, total_occ_time[i] % MINS}) << endl;

    return 0;
}