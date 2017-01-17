#include <iostream>
#include "CORERobotLib.h"
#include "WPILib.h"


using namespace CORE;
using namespace std;

/*
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
*/

class DriveSubsystem : public CORESubsystem/*, public CORESwerve*/ {
private:
    COREMotor driveMotorFL;
    COREMotor steerMotorFL;
    COREMotor driveMotorBL;
    COREMotor steerMotorBL;
    COREMotor driveMotorBR;
    COREMotor steerMotorBR;
    COREMotor driveMotorFR;
    COREMotor steerMotorFR;
    CORESwerve::SwerveModule moduleFL, moduleBL, moduleBR, moduleFR;
    CORESwerve* swerve;
public:
    DriveSubsystem() : driveMotorFL(DRIVE_FL), steerMotorFL(STEER_FL),
                       driveMotorBL(DRIVE_BL), steerMotorBL(STEER_BL),
                       driveMotorBR(DRIVE_BR), steerMotorBR(STEER_BR),
                       driveMotorFR(DRIVE_FR), steerMotorFR(STEER_FR),
                       moduleFL(&driveMotorFL, &steerMotorFL),
                       moduleBL(&driveMotorBL, &steerMotorBL),
                       moduleBR(&driveMotorBR, &steerMotorBR),
                       moduleFR(&driveMotorFR, &steerMotorFR) {
    	swerve = new CORESwerve(21, 31, moduleFL, moduleBL, moduleBR, moduleFR);
    }

    void robotInit() {
    	SmartDashboard::PutNumber("P Value", 0.004);
    	SmartDashboard::PutNumber("I Value", 0.00);
    	SmartDashboard::PutNumber("D Value", 0.00);
    	Robot::motor(STEER_FL)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEER_BL)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEER_BR)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEER_FR)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEER_BL)->setReversed();
    	Robot::motor(STEER_BR)->setReversed();
    	//Robot::motor(DRIVE_BL)->setReversed();
    	//Robot::motor(DRIVE_BR)->setReversed();
    	//Robot::motor(DRIVE_FR)->setReversed();
    }

    void teleopInit() {
    	Robot::motor(STEER_FL)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEER_BL)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEER_BR)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEER_FR)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEER_FL)->setControlMode(CONT_PID);
    	Robot::motor(STEER_BL)->setControlMode(CONT_PID);
    	Robot::motor(STEER_BR)->setControlMode(CONT_PID);
    	Robot::motor(STEER_FR)->setControlMode(CONT_PID);
    }

    void teleop() {
        swerve->cartesian(-1*Robot::joystick(0)->getAxis(LEFT_STICK_X), -1*Robot::joystick(0)->getAxis(LEFT_STICK_Y), Robot::joystick(0)->getAxis(RIGHT_STICK_X), Robot::joystick(0)->getAxis(RIGHT_TRIGGER_AXIS));

        auto x = SmartDashboard::GetNumber("P Value", 0);
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
		Robot::motor(STEER_BR)->setD(x);
		Robot::motor(STEER_FR)->setD(x);

        if(Robot::joystick(0)->getButton(DPAD_NE)){
            Robot::motor(STEER_FL)->CANTalonController->SetEncPosition(0);
            Robot::motor(STEER_BL)->CANTalonController->SetEncPosition(0);
            Robot::motor(STEER_BR)->CANTalonController->SetEncPosition(0);
            Robot::motor(STEER_FR)->CANTalonController->SetEncPosition(0);
        }

        SmartDashboard::PutNumber("FL", Robot::motor(STEER_FL)->Get());
        SmartDashboard::PutNumber("BL", Robot::motor(STEER_BL)->Get());
        SmartDashboard::PutNumber("BR", Robot::motor(STEER_BR)->Get());
        SmartDashboard::PutNumber("FR", Robot::motor(STEER_FR)->Get());

        SmartDashboard::PutNumber("FL Encoder", Robot::motor(STEER_FL)->CANTalonController->GetEncPosition());
    }

    void test() {
        cout << "Tested!" << endl;
    }
};

class arm : public COREAutoAction {
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

class wheels : public COREAutoAction {
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

class driveForwardAuto : public COREAuto {
public:
    driveForwardAuto() :
            m_moveForwardAndPutArmDown(new arm(), new wheels()), m_intakeBall(new arm()),
            m_moveBackToStart(new wheels()), m_visionAlign(new wheels()), m_shootBall(new arm()) {
        setName("Ta Da!");
        setDefault(true);
    }

    void addNodes() {
        addFirstNode(&m_moveForwardAndPutArmDown);
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
    COREJoystick joystick1;
    //driveForwardAuto auto1;
    offSeasonRobot():
            driveSubsystem(),
            joystick1(0)
            //auto1()
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
