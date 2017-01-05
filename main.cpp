#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

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

class DriveSubsystem : public CORESubsystem, public CORESwerve {
private:
    shared_ptr<COREMotor> driveMotor1, steerMotor1, driveMotor2, steerMotor2, driveMotor3, steerMotor3, driveMotor4, steerMotor4;
    CORESwerve::SwerveModule module1, module2, module3, module4;
public:
    DriveSubsystem() : driveMotor1(make_shared<COREMotor>(DRIVEMOTOR1)), steerMotor1(make_shared<COREMotor>(STEERMOTOR1)),
                       driveMotor2(make_shared<COREMotor>(DRIVEMOTOR2)), steerMotor2(make_shared<COREMotor>(STEERMOTOR2)),
                       driveMotor3(make_shared<COREMotor>(DRIVEMOTOR3)), steerMotor3(make_shared<COREMotor>(STEERMOTOR3)),
                       driveMotor4(make_shared<COREMotor>(DRIVEMOTOR4)), steerMotor4(make_shared<COREMotor>(STEERMOTOR4)),
                       module1(driveMotor1, steerMotor1, steerMotor1),
                       module2(driveMotor2, steerMotor2, steerMotor2),
                       module3(driveMotor3, steerMotor3, steerMotor3),
                       module4(driveMotor4, steerMotor4, steerMotor4),
                       CORESwerve(21, 31, module1, module2, module3, module4) {
        Robot::addMotor(driveMotor1);
        Robot::addMotor(driveMotor2);
        Robot::addMotor(driveMotor3);
        Robot::addMotor(driveMotor4);
        Robot::addMotor(steerMotor1);
        Robot::addMotor(steerMotor2);
        Robot::addMotor(steerMotor3);
        Robot::addMotor(steerMotor4);
    }

    void robotInit() {

    }

    void teleopInit() {

    }

    void teleop() {
        //cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
        Robot::joystick(0)->setAxis(LEFT_STICK_X, 1);
        Robot::joystick(0)->setAxis(LEFT_STICK_Y, 1);
        Robot::joystick(0)->setAxis(RIGHT_STICK_X, 1);
        Robot::joystick(0)->setAxis(RIGHT_STICK_Y, 1);
        cartesian(1, 0, 0);
//        cout << "Steer Motor: " << Robot::motor(STEERMOTOR)->Get() << endl;
//        cout << "Drive Motor: " << Robot::motor(DRIVEMOTOR)->Get() << endl;
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