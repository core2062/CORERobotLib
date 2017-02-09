#include "main.h"

#ifdef __arm__
START_ROBOT_CLASS(Robot)
#else
START_SIMULATED_ROBOT_CLASS(Robot)
#endif