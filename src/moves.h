#include <string>
#include "types.h"

struct Move
{
    Name name;
    Power power;
    Accuracy accuracy;
    Move(Name name, Power power, Accuracy accuracy) : name(name), power(power), accuracy(accuracy){}
};