#pragma once

#include "COREFramework/CORETask.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/COREMath.h"

namespace CORE {
    class COREDrive : public CORETask {
    public:
        COREDrive();
        virtual void setRotation(double rotation);
        virtual double getRotation();
        virtual void setThottle(double throttle);
        virtual double getThrottle();
        virtual void cartesian(double x, double y, double rotation);
        virtual void cartesian(double x, double y, double rotation, double throttle);
        virtual void setX(double x);
        virtual void setY(double y);
        virtual double getX();
        virtual double getY();
        virtual void polar(double magnitude, double direction, double rotation);
        virtual void polar(double magnitude, double direction, double rotation, double throttle);
        virtual void setMagnitude(double magnitude);
        virtual void setDirection(double direction);
        virtual double getMagnitude();
        virtual double getDirection();
        void preLoopTask() override;
        void postLoopTask() override;
    protected:
        virtual void update() = 0;
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
