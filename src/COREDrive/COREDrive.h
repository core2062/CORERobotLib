#pragma once

#include "COREFramework/CORETask.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/COREMath.h"

namespace CORE {
    class COREDrive : public CORETask {
    public:
        COREDrive();
        virtual void SetRotation(double rotation);
        virtual double GetRotation();
        virtual void SetThottle(double throttle);
        virtual double GetThrottle();
        virtual void Cartesian(double x, double y, double rotation);
        virtual void Cartesian(double x, double y, double rotation, double throttle);
        virtual void SetX(double x);
        virtual void SetY(double y);
        virtual double GetX();
        virtual double GetY();
        virtual void Polar(double magnitude, double direction, double rotation);
        virtual void Polar(double magnitude, double direction, double rotation, double throttle);
        virtual void SetMagnitude(double magnitude);
        virtual void SetDirection(double direction);
        virtual double GetMagnitude();
        virtual double GetDirection();
        void PreLoopTask() override;
        void PostLoopTask() override;
    protected:
        virtual void Update() = 0;
        double m_direction, m_x, m_y, m_rot;
    private:
        double /*m_rot, */m_mag, m_throttle;
        bool m_useThrottle, m_useCartesian, m_usePolar, m_useMag, m_useDirection;

    };
}

class TankDrive {
public:
    //TankDrive(COREMotor * frontLeft, COREMotor * backLeft, COREMotor * frontRight, COREMotor * backRight);
    void SetLeftSpeed();
    void SetRightSpeed();
    void SetSpeed();

};

class MecanumDrive {
private:

public:
    MecanumDrive();
};
