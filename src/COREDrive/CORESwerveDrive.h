#pragma once

#include <vector>

#include "COREFramework/COREHardwareManager.h"
#include "COREDrive.h"
#include "ctre/Phoenix.h"
#include "COREHardware/CORESensor.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREVector.h"

namespace CORE {
    class CORESwerve/* : public COREDrive*/ {
    public:
        class SwerveModule {
        public:
            SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor, double angleOffset = 0);
            void drive(double magnitude, double direction, double dt = -1);
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
            double m_angleOffset;
        };

        CORESwerve(double trackWidth, double wheelBase, double wheelDiameter,
        		   double ticksToRotation,
                   SwerveModule *leftFrontModule,
                   SwerveModule *leftBackModule,
                   SwerveModule *rightBackModule,
                   SwerveModule *rightFrontModule);
    public:
        void calculate(double speed, double strafeRight, double rotateClockwise);
        void calculateInverseKinematics(double fudgefactor);
        void tank(double speed, double rotateClockwise);
        string print();
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

        double leftFrontModuleSpeed = 0.0;
        double rightFrontModuleSpeed = 0.0;
        double leftBackModuleSpeed = 0.0;
        double rightBackModuleSpeed = 0.0;
        double leftFrontModuleAngle = 0.0;
        double rightFrontModuleAngle = 0.0;
        double rightBackModuleAngle = 0.0;
        double leftBackModuleAngle = 0.0;

    private:
        double m_wheelbase;
        double m_trackwidth;
        double m_ticksToRotation;
        double m_wheelCircumference;
        SwerveModule* m_leftFrontModule, *m_leftBackModule, *m_rightBackModule, *m_rightFrontModule;
    };
}


