#pragma once

namespace CORE {
    class COREDrive {
    public:
        virtual void setRotation(double rotation) = 0;
        virtual double getRotation() = 0;
        virtual void setThottle(double throttle) {}
        virtual double getThrottle() {}
        virtual void cartesian(double x, double y, double rotation) = 0;
        virtual void cartesian(double x, double y, double rotation, double throttle) {}
        virtual void setX(double x) = 0;
        virtual void setY(double y) = 0;
        virtual double getX() = 0;
        virtual double getY() = 0;
        virtual void polar(double magnitude, double direction, double rotation) = 0;
        virtual void polar(double magnitude, double direction, double rotation, double throttle) {}
        virtual void setMagnitude(double magnitude) = 0;
        virtual void setDirection(double direction) = 0;
        virtual double getMagnitude() = 0;
        virtual double getDirection() = 0;
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

class AetherDrive {
private:

public:
    AetherDrive();

};