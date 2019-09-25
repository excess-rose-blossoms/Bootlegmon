#include <string>
#include "types.h"

struct Move
{
    Power power;
    Accuracy accuracy;
    Move(Power power, Accuracy accuracy) : power(power), accuracy(accuracy){}
};