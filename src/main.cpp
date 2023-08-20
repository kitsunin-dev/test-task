#include "funcs.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    // сначала проводим юнит-тесты
    tests();

    std::ifstream file;

    // пробуем открыть файл из аргумента
    file.open(argv[1], std::ios::in);

    if (file.is_open())
        process_file(file); // если все ок, проводим обработку
    else
    {
        std::cerr << "Unable to open text file" << std::endl;
        return 1;
    }

    file.close();
    return 0;
}