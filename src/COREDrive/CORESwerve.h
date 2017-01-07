#pragma once

#include <vector>
#include <cmath>

#include <COREHardware.h>
#include "COREDrive.h"
#include "COREHardware/CORESensor.h"

namespace CORE {
enum portAssignments {
    STEERFL = 11,
    DRIVEFL = 10,
    STEERBL = 14,
    DRIVEBL = 13,
    STEERBR = 12,
    DRIVEBR = 15,
    STEERFR = 17,
    DRIVEFR = 16
};

	class CORESwerve : public CORETask, public COREDrive {
    public:
        class SwerveModule : public COREContinuous{ //TODO: SwerveModule isn't a COREContinuous, shouldn't inherit from it
        public:
            SwerveModule(COREMotor* driveMotor, COREMotor* steerMotor, COREEncoder* steerEncoder) :
		    position(0,0),
                    m_driveMotor(driveMotor), m_steerMotor(steerMotor), m_steerEncoder(steerEncoder){
                m_steerMotor->setControlMode(POS_PID);
            }

            double getCurrentAngle() {
                return clamp(m_steerMotor->CANTalonController->GetEncPosition() * m_ticksToRotations);
            }

            double getSetValue(double angle) {
                int direction = m_steerMotor->CANTalonController->GetEncPosition() == 0 ? 0 : m_steerMotor->CANTalonController->GetEncPosition() / abs(m_steerMotor->CANTalonController->GetEncPosition());
                double base = (abs(m_steerMotor->CANTalonController->GetEncPosition()) -
                               (int)(abs(m_steerMotor->CANTalonController->GetEncPosition())) % (int)(360.0 / m_ticksToRotations));
                return /*(base * direction) +*/ (angle / m_ticksToRotations);

                /*
                 int direction = encoder->Get()/abs(encoder->Get());
				double base = (abs(encoder->Get()) - (abs(encoder->Get())) % (int)(360/encoderRatio));
				return base * direction + angle/encoderRatio;
                */
            }

            double getMagnitude() {
                return m_setMagnitude;
            }

            void setMagnitude(double setMagnitude) {
                m_setMagnitude = setMagnitude;
            }


/*            double getAngle() {
                return m_setAngle;
            }

            void setAngle(double setAngle) {
                m_setAngle = setAngle;
            }*/

            void update() {
                m_driveMotor->Set(m_setMagnitude*m_setDirection);
                m_steerMotor->setPos(getSetValue(m_setAngle));
                cout << "Set Angle: " << m_setAngle << endl;
            }
            int m_setDirection = 1;
            double m_setAngle = 0;
            double m_setMagnitude = 0;
            Vector position;

        private:
            COREMotor* m_driveMotor;
            COREMotor* m_steerMotor;
            COREEncoder* m_steerEncoder;
            double m_ticksToRotations = (360 / 1024.0) * (1 / (4 * 4.22)); //TODO: This needs to have a getter + setter
        };

        CORESwerve(double trackWidth, double wheelBase, CORESwerve::SwerveModule& leftFrontModule,
                   CORESwerve::SwerveModule& leftBackModule, CORESwerve::SwerveModule& rightBackModule,
                   CORESwerve::SwerveModule& rightFrontModule);
        void setRotation(double rotation);
        double getRotation();
        void setThrottle(double throttle);
        double getThrottle();
        void cartesian(double x, double y, double rotation);
        void cartesian(double x, double y, double rotation, double throttle);
        void setX(double x);
        void setY(double y);
        double getX();
        double getY();
        void polar(double magnitude, double direction, double rotation);
        void polar(double magnitude, double direction, double rotation, double throttle);
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
        shared_ptr<SwerveModule> m_leftFrontModule, m_leftBackModule, m_rightBackModule, m_rightFrontModule;
    };
}
