#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
    STEERMOTOR = 12,
    DRIVEMOTOR = 13
};

class DriveSubsystem : public CORESubsystem {
public:
    DriveSubsystem() {

    }

    void robotInit() {

    }
    void teleopInit() {
    }
    void teleop() {
        //cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
        Robot::motor(DRIVEMOTOR)->Set(Robot::joystick(0)->getAxis(RIGHT_STICK_Y));
        Robot::motor(STEERMOTOR)->Set(Robot::joystick(0)->getAxis(LEFT_STICK_Y));
        cout << "Steer Motor: " << Robot::motor(STEERMOTOR)->Get() << endl;
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
            m_moveForwardAndPutArmDown(new arm(), new wheels()),
            m_intakeBall(new arm()),
            m_moveBackToStart(new wheels()),
            m_visionAlign(new wheels()),
            m_shootBall(new arm()) {
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
    //DriveSubsystem driveSubsystem;
    COREJoystick joystick1;
    //COREMotor driveMotor;
    //COREMotor steerMotor;
    driveForwardAuto auto1;
    offSeasonRobot():
    //driveSubsystem(),
            joystick1(0),
            //driveMotor(DRIVEMOTOR),
            //steerMotor(STEERMOTOR),
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