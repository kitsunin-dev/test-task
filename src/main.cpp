#include "funcs.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    // сначала проводим юнит-тесты
    tests();

    if (argc < 2)
    {
        std::cerr << "You did not provide the file name" << std::endl;
        return 1;
    }

    // пробуем открыть файл из аргумента
    std::ifstream file;
    file.open(argv[1], std::ios::in);

    if (!file.is_open())
    {
        std::cerr << "Unable to open text file" << std::endl;
        return 1;
    }

    // если все ок, проводим обработку
    process_file(file);

    file.close();
    return 0;
}