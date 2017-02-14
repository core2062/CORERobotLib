#pragma once

#include <map>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREEncoder.h"
#include "COREHardware/COREJoystick.h"
#include "COREHardware/CORESensor.h"

class COREHardwareManager {
public:
    static void updateMotors();
    static void updateEncoders();
    static void zeroMotors();
    static void zeroEncoders();
    static void addMotor(COREMotor* motor);
    static void addEncoder(COREEncoder* encoder);
private:
    static vector<COREMotor*> m_motors;
    static vector<COREEncoder*> m_encoders;
};
