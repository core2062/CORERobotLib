#pragma once

#include "CORERobotLib.h"
#include "AHRS.h"
#include "CANTalon.h"
#include <memory>

/****************************************************CAN TALONS********************************************************/
#define RFD_DRIVE_MOTOR_PORT 17
#define RFS_DRIVE_MOTOR_PORT 61
#define LFD_DRIVE_MOTOR_PORT 60
#define LFS_DRIVE_MOTOR_PORT 59
#define RBD_DRIVE_MOTOR_PORT 10
#define RBS_DRIVE_MOTOR_PORT 11
#define LBD_DRIVE_MOTOR_PORT 13
#define LBS_DRIVE_MOTOR_PORT 12

#define LEFT_CLIMB_MOTOR_PORT 14
#define RIGHT_CLIMB_MOTOR_PORT 15
//#define LIFT_MOTOR_PORT 16

/****************************************************PWM MOTORS********************************************************/
#define LIFT_MOTOR_PORT 0
#define INTAKE_MOTOR_PORT 1
#define LEFT_DUMP_FLAP_SERVO_CHANNEL 2
#define RIGHT_DUMP_FLAP_SERVO_CHANNEL 3

/****************************************************ANALOG SENSORS****************************************************/
#define LIFT_ENCODER_A_PORT 0
#define LIFT_ENCODER_B_PORT 1
#define LEFT_GEAR_ULTRA_PORT 2
#define RIGHT_GEAR_ULTRA_PORT 3

/****************************************************DIGITAL SENSORS***************************************************/
#define CLIMB_LIMIT_SWITCH_PORT 0
#define GEAR_MANIPULATOR_LIMIT_SWITCH_PORT 1
#define LIFT_BOTTOM_LIMIT_SWITCH_PORT 2
#define LIFT_TOP_LIMIT_SWITCH_PORT 3

/****************************************************SOLENOIDS*********************************************************/
#define LEFT_DRIVE_SHIFTER_HIGH_GEAR_PORT 0
#define LEFT_DRIVE_SHIFTER_LOW_GEAR_PORT 1
#define LEFT_DRIVE_SHIFTER_PCM 1
#define RIGHT_DRIVE_SHIFTER_HIGH_GEAR_PORT 2
#define RIGHT_DRIVE_SHIFTER_LOW_GEAR_PORT 3
#define RIGHT_DRIVE_SHIFTER_PCM 1
#define LEFT_GEAR_FLAP_SOLENOID_OUT_PORT 0
#define LEFT_GEAR_FLAP_SOLENOID_IN_PORT 1
#define LEFT_GEAR_FLAP_SOLENOID_PCM 2
#define RIGHT_GEAR_FLAP_SOLENOID_OUT_PORT 2
#define RIGHT_GEAR_FLAP_SOLENOID_IN_PORT 3
#define RIGHT_GEAR_FLAP_SOLENOID_PCM 2

#define GEAR_PICKUP_LEFT_OUT_PORT 4
#define GEAR_PICKUP_LEFT_IN_PORT 5
#define GEAR_PICKUP_LEFT_PCM 2
#define GEAR_PICKUP_RIGHT_OUT_PORT 4
#define GEAR_PICKUP_RIGHT_IN_PORT 5
#define GEAR_PICKUP_RIGHT_PCM 1
#define GEAR_PICKUP_FORWARD_PCM 1
#define GEAR_PICKUP_FORWARD_ON_PORT 4
#define GEAR_PICKUP_FORWARD_OFF_PORT 5
#define GEAR_PICKUP_REVERSE_PCM 2
#define GEAR_PICKUP_REVERSE_ON_PORT 4
#define GEAR_PICKUP_REVERSE_OFF_PORT 5
#define GEAR_INTAKE_PORT 16

using namespace std;
using namespace CORE;

class DriveSubsystem : public CORESubsystem {
public:
    DriveSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
//private:
    CANTalon m_leftFrontSteer, m_rightFrontSteer, m_leftBackSteer, m_rightBackSteer,
            m_rightFrontDrive, m_leftFrontDrive, m_rightBackDrive, m_leftBackDrive;
    CORESwerve::SwerveModule* m_rightFrontModule;
    CORESwerve::SwerveModule* m_rightBackModule;
    CORESwerve::SwerveModule* m_leftFrontModule;
    CORESwerve::SwerveModule* m_leftBackModule;
    CORESwerve m_swerveDrive;
    shared_ptr<AHRS> m_gyro;
};

