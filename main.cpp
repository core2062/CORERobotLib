#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
    STEERMOTOR = 12,
    DRIVEMOTOR = 13
};

class DriveSubsystem : public CORESubsystem {
private:
    SwerveDrive::SwerveModule module1;
    vector<shared_ptr<SwerveDrive::SwerveModule>> m_modules;
public:
        SwerveDrive* swerve;
        DriveSubsystem() : module1(Robot::motor(DRIVEMOTOR), Robot::motor(STEERMOTOR), Robot::motor(DRIVEMOTOR)) {
            //shared_ptr<SwerveDrive::SwerveModule> pointer(&module1);
            //m_modules[0] = pointer;
            //swerve = new SwerveDrive(m_modules);
        }
    void robotInit() {

    }

    void teleopInit() {

    }

    void teleop() {
        //cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
        Robot::joystick(0)->setAxis(LEFT_STICK_X, 0);
        Robot::joystick(0)->setAxis(LEFT_STICK_Y, 0);
        Robot::joystick(0)->setAxis(RIGHT_STICK_X, 0);
        Robot::joystick(0)->setAxis(RIGHT_STICK_Y, 0);
        //swerve->run(0,0,0);
        cout << "Steer Motor: " << Robot::motor(STEERMOTOR)->Get() << endl;
        cout << "Drive Motor: " << Robot::motor(DRIVEMOTOR)->Get() << endl;
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
    COREMotor driveMotor;
    COREMotor steerMotor;
    //driveForwardAuto auto1;
    offSeasonRobot():
            driveSubsystem(),
            joystick1(0),
            driveMotor(DRIVEMOTOR),
            steerMotor(STEERMOTOR)//,
            //auto1()
    {
        setLoopTime(0.5);

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