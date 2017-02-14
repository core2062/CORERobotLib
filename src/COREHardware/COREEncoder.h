#pragma once

#include <iostream>
#include <memory>

#include <WPILib.h>
#include "CORETimer.h"
#include "CANTalon.h"
#include <COREController.h>

using namespace std;

namespace CORE {
    enum distanceUnit {
        FEET,
        INCHES,
        CENTIMETERS,
        METERS
    };

    enum encoderType {
        QUAD,
        SRX_RELATIVE,
        SRX_ABSOLUTE,
        ANALOG_POT
    };
    class COREEncoder : public ControllerInput {
    public:
        shared_ptr<Encoder> encoder;
        shared_ptr<CANTalon> CANTalonController;
        COREEncoder(shared_ptr<CANTalon> boundCANTalonController, encoderType encoderType, bool reversed = false);
        COREEncoder(int portA, int portB, encoderType encoder, bool reversed = false);
        void setEncReversed(bool reversed = true);
        bool getEncReversed();
        int GetEncPos();
        double GetEncVel();
        double GetEncAccel();
        double getDistance(distanceUnit unit = FEET);
        void setDistance(distanceUnit unit, double ticksToUnit);
        void SetEncPos(int value);
        void Reset();
        int GetPortA();
        int GetPortB();
        bool IsBoundToCANTalon();
        void Update();
    protected:
        CORETimer m_timer;
        bool m_boundToCANTalon = false;
        double m_ticksToDistanceConversion[4];
        int m_pos;
        double m_vel, m_accel, m_lastVel;
        int m_lastPos;
        encoderType m_encoderType;
        int m_portA, m_portB;
        bool m_reversed;
        int m_offset;
    };
}
