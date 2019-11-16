#include <string>
#include "types.h"

struct Move
{
    Name name;
    Power power;
    Accuracy accuracy;
    Move(Name name, Power power, Accuracy accuracy) : name(name), power(power), accuracy(accuracy){}
};

// Used for prototyping
struct SimpleMove
{
    Name name;
    Power power;
    SimpleMove(Name name, Power power): name(name), power(power){}
};