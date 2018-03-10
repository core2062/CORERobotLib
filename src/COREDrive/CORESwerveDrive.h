#pragma once

#include <vector>

#include "COREFramework/COREHardwareManager.h"
#include "COREUtilities/COREConstant.h"
#include "COREDrive.h"
#include "ctre/Phoenix.h"
#include "COREHardware/CORESensor.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREVector.h"
#include "COREUtilities/Position2d.h"

namespace CORE {
    class CORESwerve/* : public COREDrive*/ {
    public:
        class SwerveModule {
        public:
            SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor);
            void drive(COREVector vector, double dt = -1);
            COREVector inverseKinematics(double wheelCircumference, double ticksToRotation);
            double getAngle(bool raw = false);
            double getTotalTicks();
            void setAnglePID(double p, double i, double d);
            void setAngleOffset(double angleOffset);
            void zeroAngle();
            string print();
            COREPID m_speedPIDController;
            COREAnglePID m_anglePIDController;
        private:
            TalonSRX *m_driveMotor;
            TalonSRX *m_steerMotor;
            double m_lastMagnitude = 0.0;
            double m_lastAngle = 0.0;
            double m_angleOffset;
        };

        CORESwerve(double trackWidth, double wheelBase, double wheelDiameter,
        		   double ticksToRotation,
                   SwerveModule *leftFrontModule,
                   SwerveModule *leftBackModule,
                   SwerveModule *rightBackModule,
                   SwerveModule *rightFrontModule);
    public:
        void calculate(double y, double x, double theta);
        COREVector inverseKinematics(double gyroAngle);
        void tank(double speed, double rotateClockwise);
        void update(double dt = -1);
        void setSteerPID(double kp, double ki, double kd);
        void zeroOffsets();

        double rightFrontDeltaX = 0.0;
        double rightFrontDeltaY = 0.0;
        double leftFrontDeltaX = 0.0;
        double leftFrontDeltaY = 0.0;
        double rightBackDeltaX = 0.0;
        double rightBackDeltaY = 0.0;
        double leftBackDeltaX = 0.0;
        double leftBackDeltaY = 0.0;
    private:
        double m_wheelbase;
        double m_trackwidth;
        double m_ticksToRotation;
        double m_wheelCircumference;
        SwerveModule* m_frontLeftModule, *m_backLeftModule, *m_backRightModule, *m_frontRightModule;
        COREConstant<double> m_leftFrontModuleOffset, m_leftBackModuleOffset, m_rightBackModuleOffset,
                m_rightFrontModuleOffset;
        COREVector m_frontLeft, m_frontRight, m_backLeft, m_backRight;
    };
}


