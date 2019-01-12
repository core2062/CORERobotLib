#pragma once

#include <vector>

#include "COREFramework/COREHardwareManager.h"
#include "COREUtilities/COREConstant.h"
#include "COREDrive.h"
#include "COREHardware/CORESensor.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREVector.h"
#include "COREDrive/COREEtherDrive.h"

namespace CORE {
    class CORESwerve/* : public COREDrive*/ {
    public:
        class SwerveModule {
        public:
            SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor);
            void Drive(COREVector vector, double dt = -1);
            COREVector ForwardKinematics(double wheelCircumference, double ticksToRotation);
            double GetAngle(bool raw = false);
            double GetEncoder();
            void SetAnglePID(double p, double i, double d);
            void SetAngleOffset(double angleOffset);
            void ZeroAngle();
            void ZeroEncoder();
            string Print();
            COREPID m_speedPIDController;
            COREAnglePID m_anglePIDController;
        private:
            TalonSRX *m_driveMotor;
            TalonSRX *m_steerMotor;
            double m_lastMagnitude = 0.0;
            double m_angleOffset = 0.0;
        };

        CORESwerve(double trackWidth, double wheelBase, double wheelDiameter,
        		   double ticksToRotation,
                   SwerveModule *leftFrontModule,
                   SwerveModule *leftBackModule,
                   SwerveModule *rightBackModule,
                   SwerveModule *rightFrontModule);
        void InverseKinematics(double x, double y, double theta);
        Position2d ForwardKinematics();
        void Brake();
        void SetSteerPID(double kp, double ki, double kd);
        void ZeroOffsets();
        void ZeroEncoders();
        void UpdateOffsets();
    private:
        double m_wheelbase;
        double m_trackwidth;
        double m_ticksToRotation;
        double m_wheelCircumference;
        SwerveModule* m_frontLeftModule, *m_backLeftModule, *m_backRightModule, *m_frontRightModule;
        COREConstant<double> m_leftFrontModuleOffset, m_leftBackModuleOffset, m_rightBackModuleOffset,
                m_rightFrontModuleOffset;
        COREVector m_frontLeft, m_frontRight, m_backLeft, m_backRight;
        CORETimer m_timer;
    };
}