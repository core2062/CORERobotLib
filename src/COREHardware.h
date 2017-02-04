#pragma once

#include <map>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREEncoder.h"
#include "COREHardware/COREJoystick.h"
#include "COREHardware/CORESensor.h"

class COREHardwareManager {
    friend class COREScheduler;
    friend class COREMotor;
public:
    static void updateMotors();
private:
    static void addMotor(COREMotor * motor);
    static vector<COREMotor*> m_motors;
};