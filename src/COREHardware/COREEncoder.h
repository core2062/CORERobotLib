//#pragma once
//
//#include <iostream>
//#include <memory>
//
////#include <WPILib.h>
//#include "COREUtilities/CORETimer.h"
//#include "COREFramework/COREHardwareManager.h"
//#include "TalonSRX.h"
//#include "COREControl/COREController.h"
//
//using namespace std;
//
//namespace CORE {
//    enum distanceUnit {
//        FEET,
//        INCHES,
//        CENTIMETERS,
//        METERS
//    };
//
//    enum encoderType {
//        QUAD,
//        SRX_RELATIVE,
//        SRX_ABSOLUTE,
//        ANALOG_POT
//    };
//    class COREEncoder : public ICOREEncoder, public ControllerInput {
//    public:
////        shared_ptr<Encoder> encoder;
//        shared_ptr<TalonSRX> TalonSRXController;
//        COREEncoder(shared_ptr<TalonSRX> boundTalonSRXController, encoderType encoderType, bool reversed = false);
//        COREEncoder(int portA, int portB, encoderType encoder, bool reversed = false);
//        void setEncReversed(bool reversed = true);
//        bool getEncReversed();
//        int GetEncPos();
//        double GetEncVel();
//        double GetEncAccel();
//        double getDistance(distanceUnit unit = FEET);
//        void setDistance(distanceUnit unit, double ticksToUnit);
//        void SetEncPos(int value);
//        void Reset();
//        int getPortA();
//        int getPortB();
//        bool isBoundToTalonSRX();
//        void Update();
//        int getTalonSRXID() override;
//    protected:
//        CORETimer m_timer;
//        bool m_boundToTalonSRX = false;
//        double m_ticksToDistanceConversion[4];
//        int m_pos;
//        double m_vel, m_accel, m_lastVel;
//        int m_lastPos;
//        encoderType m_encoderType;
//        int m_portA, m_portB;
//        bool m_reversed;
//        int m_offset;
//    };
//}
