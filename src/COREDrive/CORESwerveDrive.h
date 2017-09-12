//TODO: Rewrite everything

#pragma once

#include <vector>

#include "COREFramework/COREHardwareManager.h"
#include "COREDrive.h"
#include "CANTalon.h"
#include "../COREHardware/CORESensor.h"
#include "COREControl/COREPID.h"
#include "COREEtherDrive.h"

namespace CORE {
    class CORESwerve : public COREDrive {
    public:
        class SwerveModule{
        public:
            SwerveModule(CANTalon* driveMotor, CANTalon* steerMotor) :
                    m_speedPIDController(0,0,0),
                    m_anglePIDController(0,0,0),
                    m_driveMotor(driveMotor),
                    m_steerMotor(steerMotor) {
            }
            void drive(double magnitude, double direction);
            double getAngle();
            void setAnglePID(double p, double i, double d);
            COREPID m_speedPIDController;
            COREAnglePID m_anglePIDController;
        private:
            CANTalon* m_driveMotor;
            CANTalon* m_steerMotor;

        };
        CORESwerve(double trackWidth, double wheelBase,
                   CORESwerve::SwerveModule &leftFrontModule,
                   CORESwerve::SwerveModule &leftBackModule,
                   CORESwerve::SwerveModule &rightBackModule,
                   CORESwerve::SwerveModule &rightFrontModule);
    public:
        void calculate(double speed, double strafeRight, double rotateClockwise);
        void tank(double speed, double rotateClockwise);
        void tank(VelocityPair speeds);
        void update();
        double leftFrontModuleSpeed = 0.0;
        double rightFrontModuleSpeed = 0.0;
        double leftBackModuleSpeed = 0.0;
        double rightBackModuleSpeed = 0.0;
        double leftFrontModuleAngle = 0.0;
        double rightFrontModuleAngle = 0.0;
        double rightBackModuleAngle = 0.0;
        double leftBackModuleAngle = 0.0;

    protected:
        double m_wheelbase = 22.3;
        double m_trackwidth = 27.7;
    private:
        vector<shared_ptr<SwerveModule>> m_modules;
        shared_ptr<SwerveModule> m_leftFrontModule, m_leftBackModule, m_rightBackModule, m_rightFrontModule;
    };
}


