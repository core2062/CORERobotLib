#include <iostream>
#include "CORERobotLib.h"
#include "WPILib.h"


using namespace CORE;
using namespace std;

enum portAssignments {
    STEER_FL = 11,
    DRIVE_FL = 10,
    STEER_BL = 14,
    DRIVE_BL = 13,
    STEER_BR = 12,
    DRIVE_BR = 15,
    STEER_FR = 17,
    DRIVE_FR = 16
};

class DriveSubsystem : public CORESubsystem {
private:
    COREMotor driveMotorFL;
    COREMotor steerMotorFL;
    COREMotor driveMotorBL;
    COREMotor steerMotorBL;
    COREMotor driveMotorBR;
    COREMotor steerMotorBR;
    COREMotor driveMotorFR;
    COREMotor steerMotorFR;
    COREJoystick joystick1;
    COREConstant<string> testing;
public:
    DriveSubsystem() : CORESubsystem("Drive Subsystem"),
                       driveMotorFL(DRIVE_FL), steerMotorFL(STEER_FL),
                       driveMotorBL(DRIVE_BL), steerMotorBL(STEER_BL),
                       driveMotorBR(DRIVE_BR), steerMotorBR(STEER_BR),
                       driveMotorFR(DRIVE_FR), steerMotorFR(STEER_FR),
                       joystick1(0),
					   testing("Test", "Test Value") {
    }

    void robotInit() {
    	SmartDashboard::PutNumber("P Value", 0.004);
    	SmartDashboard::PutNumber("I Value", 0.00);
    	SmartDashboard::PutNumber("D Value", 0.00);
        steerMotorFL.CANTalonController->SetEncPosition(0);
        steerMotorBL.CANTalonController->SetEncPosition(0);
        steerMotorBR.CANTalonController->SetEncPosition(0);
        steerMotorFR.CANTalonController->SetEncPosition(0);
        steerMotorBL.setReversed();
        steerMotorBR.setReversed();
    	//Robot::motor(DRIVE_BL)->setReversed();
    	//Robot::motor(DRIVE_BR)->setReversed();
    	//Robot::motor(DRIVE_FR)->setReversed();
    }

    void teleopInit() {
    	steerMotorFL.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	steerMotorBL.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	steerMotorBR.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	steerMotorFR.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
/*    	Robot::motor(STEER_FL)->setControlMode(CONT_PID);
    	Robot::motor(STEER_BL)->setControlMode(CONT_PID);
    	Robot::motor(STEER_BR)->setControlMode(CONT_PID);
    	Robot::motor(STEER_FR)->setControlMode(CONT_PID);*/
    }

    void teleop() {

/*        auto x = SmartDashboard::GetNumber("P Value", 0);
        Robot::motor(STEER_FL)->setP(x);
        Robot::motor(STEER_BL)->setP(x);
        Robot::motor(STEER_BR)->setP(x);
        Robot::motor(STEER_FR)->setP(x);

        x = SmartDashboard::GetNumber("I Value", 0);
		Robot::motor(STEER_FL)->setI(x);
		Robot::motor(STEER_BL)->setI(x);
		Robot::motor(STEER_BR)->setI(x);
		Robot::motor(STEER_FR)->setI(x);

		x = SmartDashboard::GetNumber("D Value", 0);
		Robot::motor(STEER_FL)->setD(x);
		Robot::motor(STEER_BL)->setD(x);
		Robot::`motor(STEER_BR)->setD(x);
		Robot::motor(STEER_FR)->setD(x);*/

        if(joystick1.getButton(COREJoystick::DPAD_NE)){
            steerMotorFL.CANTalonController->SetEncPosition(0);
            steerMotorBL.CANTalonController->SetEncPosition(0);
            steerMotorBR.CANTalonController->SetEncPosition(0);
            steerMotorFR.CANTalonController->SetEncPosition(0);
        }

        SmartDashboard::PutNumber("FL", steerMotorFL.Get());
        SmartDashboard::PutNumber("BL", steerMotorBL.Get());
        SmartDashboard::PutNumber("BR", steerMotorBR.Get());
        SmartDashboard::PutNumber("FR", steerMotorFR.Get());

        SmartDashboard::PutNumber("FL Encoder", steerMotorFL.CANTalonController->GetEncPosition());
    }

    void test() {
        cout << "Tested!" << endl;
    }
};

class arm : public COREAutonAction {
public:
    actionStatus action() {
        cout << "Arm!: " << count << endl;
        if(count < 3) {
            count++;
            return actionStatus::CONTINUE;
        }
        else {
            return actionStatus::END;
        }
    }

private:
    int count = 0;
};

class wheels : public COREAutonAction {
public:
    actionStatus action() {
        cout << "Wheels!: " << count << endl;
        if(count < 3) {
            count++;
            return actionStatus::CONTINUE;
        }
        else {
            return actionStatus::END;
        }
    }

private:
    int count = 0;
};

class driveForwardAuto : public COREAuton {
public:
    driveForwardAuto() : COREAuton("Ta Da!", &m_moveForwardAndPutArmDown, false),
                         m_moveForwardAndPutArmDown(new arm(), new wheels()), m_intakeBall(new arm()),
                         m_moveBackToStart(new wheels()), m_visionAlign(new wheels()), m_shootBall(new arm()) {
    }

    void addNodes() {
        m_moveForwardAndPutArmDown.addNext(&m_intakeBall);
        m_intakeBall.addNext(&m_moveBackToStart);
        m_moveBackToStart.addCondition(ballIntaked);
        m_intakeBall.addNext(&m_visionAlign);
        m_visionAlign.addCondition(targetAquired);
        m_visionAlign.addNext(&m_shootBall);
    }

private:
    Node m_moveForwardAndPutArmDown, m_intakeBall, m_moveBackToStart, m_visionAlign, m_shootBall;
    static bool ballIntaked() {
        return false;
    }

    static bool targetAquired() {
        return false;
    }
};

class offSeasonRobot : public CORERobot {
public:
    DriveSubsystem driveSubsystem;
    driveForwardAuto auto1;
    offSeasonRobot():
            driveSubsystem(),
            auto1()
    {

    }

    void robotInit() {

    }

    void teleopInit() {

    }

    void teleop() {
        cout << "\nNew Iteration: \n" << endl;
    }
};

#ifdef __arm__
START_ROBOT_CLASS(offSeasonRobot)
#else
START_SIMULATED_ROBOT_CLASS(offSeasonRobot)
#endif
