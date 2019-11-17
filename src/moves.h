#pragma once

#include <string>
#include "types.h"

struct Move
{
    int index_num;
    Name name;
    Power power;
    Accuracy accuracy;

    Move() : name(InvalidMoveName) {}
    Move(int index_num, Name name, Power power, Accuracy accuracy) : index_num(index_num), name(name), power(power), accuracy(accuracy){}

    // Prints out info about the move. Used for debugging.
    void MakeReport()
    {
        std::cout << "INDEX: " << index_num << std::endl;
        std::cout << "NAME: " << name << std::endl;
        std::cout << "POWER: " << power << std::endl;
        std::cout << "ACCURACY: " << accuracy << std::endl;
        std::cout << std::endl;
    }
};