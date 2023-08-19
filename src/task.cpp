#include "task.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    std::ifstream file;

    file.open(argv[1], std::ios::in);

    if (file.is_open())
        process_file(file);
    else
    {
        std::cerr << "Unable to open text file" << std::endl;
        return 1;
    }
    
    file.close();
    return 0;
}