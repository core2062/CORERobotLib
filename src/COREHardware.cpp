#include "COREHardware.h"

using namespace CORE;


std::map<int, std::shared_ptr<COREMotor>> CORE::COREHardware::motorMap;
std::map<int, std::shared_ptr<COREJoystick>> CORE::COREHardware::joystickMap;
