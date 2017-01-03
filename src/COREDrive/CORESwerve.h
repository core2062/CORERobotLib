#pragma once

#include <vector>
#include <cmath>

#include "COREHardware.h"
#include "COREDrive.h"
#include "COREHardware/CORESensor.h"

namespace CORE {
    class CORESwerve : public CORETask, public COREDrive {
    public:
        class SwerveModule : public COREContinuous, public Vector { //TODO: SwerveModule isn't a COREContinuous, shouldn't inherit from it
        public:
            SwerveModule(int driveMotor, int steerMotor,
                         int steerEncoder) {
                m_driveMotor = move(Robot::motor(driveMotor));
                m_steerMotor = move(Robot::motor(steerMotor));
                m_steerEncoder = move(Robot::motor(steerEncoder));
                //m_steerMotor->setControlMode(POS_PID);
//                if(m_steerMotor->getP() == 0) {
//                    cout << "Warning, steerMotor not given pid values" << endl;
//                }

            }

            double getCurrentAngle() {
                return clamp(m_steerEncoder->Get() * m_encoderRatio);
            }

            double getSetValue(double angle) {
                int direction = m_steerEncoder->Get() / abs(m_steerEncoder->Get());
                double base = (abs(m_steerEncoder->Get()) -
                               (int)(abs(m_steerEncoder->Get())) % (int)(360 / m_encoderRatio));
                return base * direction + angle / m_encoderRatio;
            }

            double getMagnitude() {
                return m_setMagnitude;
            }

            void setMagnitude(double setMagnitude) {
                m_setMagnitude = setMagnitude;
            }

            double getDirection() {
                return m_setDirection;
            }

            void setDirection(double setDirection) {
                m_setDirection = setDirection;
            }

            void update() {
                m_driveMotor->Set(m_setMagnitude);
                m_steerMotor->Set(getSetValue(m_setDirection));
            }

        private:
            shared_ptr<COREMotor> m_driveMotor;
            shared_ptr<COREMotor> m_steerMotor;
            shared_ptr<COREEncoder> m_steerEncoder;
            double m_setMagnitude = 0;
            double m_setDirection = 0;
            double m_encoderRatio = (360 / 1024.0) * (1 / (16 * 4.22)); //TODO: This needs to have a getter + setter
        };

        CORESwerve(double wheelBase, double trackWidth, shared_ptr<SwerveModule> leftFrontModule,
                   shared_ptr<SwerveModule> leftBackModule, shared_ptr<SwerveModule> rightBackModule,
                   shared_ptr<SwerveModule> rightFrontModule = nullptr);
        void setRotation(double rotation);
        double getRotation();
        void cartesian(double x, double y, double rotation);
        void setX(double x);
        void setY(double y);
        double getX();
        double getY();
        void polar(double magnitude, double direction, double rotation);
        void setMagnitude(double magnitude);
        void setDirection(double direction);
        double getMagnitude();
        double getDirection();
        void postTeleopTask();
    protected:
        bool m_forceMode = false;
        double m_wheelbase = 0.0;
        double m_trackwidth = 0.0;
        double m_rotationScalar = 1.0;
        double m_y = 0.0;
        double m_x = 0.0;
        double m_rot = 0.0;
        double m_throttle = 0.0;

    private:
        vector<shared_ptr<SwerveModule>> m_modules;
    };
}
