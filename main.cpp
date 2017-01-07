#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

/*
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
    COREEncoder none;
    CORESwerve::SwerveModule moduleFL, moduleBL, moduleBR, moduleFR;
    CORESwerve* swerve;
public:
    DriveSubsystem() : driveMotorFL(DRIVEFL), steerMotorFL(STEERFL),
                       driveMotorBL(DRIVEBL), steerMotorBL(STEERBL),
                       driveMotorBR(DRIVEBR), steerMotorBR(STEERBR),
                       driveMotorFR(DRIVEFR), steerMotorFR(STEERFR),
					   none(0, 0, encoderType::NONE),
                       moduleFL(&driveMotorFL, &steerMotorFL, &none),
                       moduleBL(&driveMotorBL, &steerMotorBL, &none),
                       moduleBR(&driveMotorBR, &steerMotorBR, &none),
                       moduleFR(&driveMotorFR, &steerMotorFR, &none) {
    	swerve = new CORESwerve(21, 31, moduleFL, moduleBL, moduleBR, moduleFR);
    }

    void robotInit() {
    	SmartDashboard::PutNumber("P Value", 0.4);
    	SmartDashboard::PutNumber("I Value", 0.01);
    	SmartDashboard::PutNumber("D Value", 0.00);
    	Robot::motor(STEERFL)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERBL)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERBR)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERFR)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERBL)->setReversed();
    	Robot::motor(STEERBR)->setReversed();
    	//Robot::motor(DRIVEBL)->setReversed();
    	//Robot::motor(DRIVEBR)->setReversed();
    	//Robot::motor(DRIVEFR)->setReversed();
    }

    void teleopInit() {
    	Robot::motor(STEERFL)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERBL)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERBR)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERFR)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    }

    void teleop() {
        //cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
        Robot::joystick(0)->setAxis(LEFT_STICK_X, 0);
        Robot::joystick(0)->setAxis(LEFT_STICK_Y, 0);
        Robot::joystick(0)->setAxis(RIGHT_STICK_X, 0);
        Robot::joystick(0)->setAxis(LEFT_TRIGGER_AXIS, 0);
        swerve->cartesian(Robot::joystick(0)->getAxis(LEFT_STICK_X), Robot::joystick(0)->getAxis(LEFT_STICK_Y), Robot::joystick(0)->getAxis(RIGHT_STICK_X));

        auto x = SmartDashboard::GetNumber("P Value", 0.0001);
        Robot::motor(STEERFL)->setP(x/10000.0);
        Robot::motor(STEERBL)->setP(x/10000.0);
        Robot::motor(STEERBR)->setP(x/10000.0);
        Robot::motor(STEERFR)->setP(x/10000.0);

        x = SmartDashboard::GetNumber("I Value", 0.0001);
		Robot::motor(STEERFL)->setI(x/10000.0);
		Robot::motor(STEERBL)->setI(x/10000.0);
		Robot::motor(STEERBR)->setI(x/10000.0);
		Robot::motor(STEERFR)->setI(x/10000.0);

		x = SmartDashboard::GetNumber("D Value", 0.0001);
		Robot::motor(STEERFL)->setD(x/10000.0);
		Robot::motor(STEERBL)->setD(x/10000.0);
		Robot::motor(STEERBR)->setD(x/10000.0);
		Robot::motor(STEERFR)->setD(x/10000.0);
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
        setLoopTime(1);
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
