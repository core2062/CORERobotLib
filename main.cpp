#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

/*
enum portAssignments {
    STEERMOTOR1 = 11,
    DRIVEMOTOR1 = 10,
    STEERMOTOR2 = 14,
    DRIVEMOTOR2 = 13,
    STEERMOTOR3 = 12,
    DRIVEMOTOR3 = 15,
    STEERMOTOR4 = 17,
    DRIVEMOTOR4 = 16
};
*/

class DriveSubsystem : public CORESubsystem/*, public CORESwerve*/ {
private:
    COREMotor driveMotor1;
    COREMotor steerMotor1;
    COREMotor driveMotor2;
    COREMotor steerMotor2;
    COREMotor driveMotor3;
    COREMotor steerMotor3;
    COREMotor driveMotor4;
    COREMotor steerMotor4;
    COREEncoder none;
    CORESwerve::SwerveModule module1, module2, module3, module4;
    CORESwerve* swerve;
public:
    DriveSubsystem() : driveMotor1(DRIVEMOTOR1), steerMotor1(STEERMOTOR1),
                       driveMotor2(DRIVEMOTOR2), steerMotor2(STEERMOTOR2),
                       driveMotor3(DRIVEMOTOR3), steerMotor3(STEERMOTOR3),
                       driveMotor4(DRIVEMOTOR4), steerMotor4(STEERMOTOR4),
					   none(0,0,encoderType::NONE),
                       module1(&driveMotor1, &steerMotor1, &none),
                       module2(&driveMotor2, &steerMotor2, &none),
                       module3(&driveMotor3, &steerMotor3, &none),
                       module4(&driveMotor4, &steerMotor4, &none) {
    	swerve = new CORESwerve(21, 31, module1, module2, module3, module4);
    }

    void robotInit() {
    	SmartDashboard::PutNumber("P Value", 0.4);
    	SmartDashboard::PutNumber("I Value", 0.01);
    	SmartDashboard::PutNumber("D Value", 0.00);
    	Robot::motor(STEERMOTOR1)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERMOTOR2)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERMOTOR3)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERMOTOR4)->CANTalonController->SetEncPosition(0);
    	Robot::motor(STEERMOTOR2)->setReversed();
    	Robot::motor(STEERMOTOR3)->setReversed();
    	//Robot::motor(DRIVEMOTOR2)->setReversed();
    	//Robot::motor(DRIVEMOTOR3)->setReversed();
    	//Robot::motor(DRIVEMOTOR4)->setReversed();
    }

    void teleopInit() {
    	Robot::motor(STEERMOTOR1)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERMOTOR2)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERMOTOR3)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERMOTOR4)->CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	Robot::motor(STEERMOTOR1)->setControlMode(POS_PID);
    	Robot::motor(STEERMOTOR2)->setControlMode(POS_PID);
    	Robot::motor(STEERMOTOR3)->setControlMode(POS_PID);
    	Robot::motor(STEERMOTOR4)->setControlMode(POS_PID);
    }

    void teleop() {
        //cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
/*        Robot::joystick(0)->setAxis(LEFT_STICK_X, 0);
        Robot::joystick(0)->setAxis(LEFT_STICK_Y, 0);
        Robot::joystick(0)->setAxis(RIGHT_STICK_X, 1);
        Robot::joystick(0)->setAxis(RIGHT_STICK_Y, 1);*/
        swerve->cartesian(Robot::joystick(0)->getAxis(LEFT_STICK_X), Robot::joystick(0)->getAxis(LEFT_STICK_Y), Robot::joystick(0)->getAxis(RIGHT_STICK_X));
//        cout << "Steer Motor: " << Robot::motor(STEERMOTOR)->Get() << endl;
//        cout << "Drive Motor: " << Robot::motor(DRIVEMOTOR)->Get() << endl;
        auto x = SmartDashboard::GetNumber("P Value", 0.0001);
        Robot::motor(STEERMOTOR1)->setP(x/10000.0);
        Robot::motor(STEERMOTOR2)->setP(x/10000.0);
        Robot::motor(STEERMOTOR3)->setP(x/10000.0);
        Robot::motor(STEERMOTOR4)->setP(x/10000.0);

        x = SmartDashboard::GetNumber("I Value", 0.0001);
		Robot::motor(STEERMOTOR1)->setI(x/10000.0);
		Robot::motor(STEERMOTOR2)->setI(x/10000.0);
		Robot::motor(STEERMOTOR3)->setI(x/10000.0);
		Robot::motor(STEERMOTOR4)->setI(x/10000.0);

		x = SmartDashboard::GetNumber("D Value", 0.0001);
		Robot::motor(STEERMOTOR1)->setD(x/10000.0);
		Robot::motor(STEERMOTOR2)->setD(x/10000.0);
		Robot::motor(STEERMOTOR3)->setD(x/10000.0);
		Robot::motor(STEERMOTOR4)->setD(x/10000.0);
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
        setLoopTime(0.01);
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
