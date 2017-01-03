#include <iostream>
#include "CORERobotLib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
    STEERMOTOR1 = 12,
    DRIVEMOTOR1 = 13,
    STEERMOTOR2 = 14,
    DRIVEMOTOR2 = 15,
    STEERMOTOR3 = 16,
    DRIVEMOTOR3 = 17,
    STEERMOTOR4 = 18,
    DRIVEMOTOR4 = 19,
};

class DriveSubsystem : public CORESubsystem {
private:
//    SwerveDrive::SwerveModule module1;
//    vector<shared_ptr<SwerveDrive::SwerveModule>> m_modules;
    shared_ptr<CORESwerve::SwerveModule> module1, module2, module3, module4;
    shared_ptr<COREMotor> driveMotor1, steerMotor1, driveMotor2, steerMotor2, driveMotor3, steerMotor3, driveMotor4, steerMotor4;
public:
//        SwerveDrive* swerve;
//        DriveSubsystem() : module1(Robot::motor(DRIVEMOTOR), Robot::motor(STEERMOTOR), Robot::motor(DRIVEMOTOR)) {
//            //shared_ptr<SwerveDrive::SwerveModule> pointer(&module1);
//            //m_modules[0] = pointer;
//            //swerve = new SwerveDrive(m_modules);
//        }
    CORESwerve* swerve;
    DriveSubsystem() : driveMotor1(new COREMotor(DRIVEMOTOR1)), steerMotor1(new COREMotor(STEERMOTOR1)),
                       driveMotor2(new COREMotor(DRIVEMOTOR2)), steerMotor2(new COREMotor(STEERMOTOR2)),
                       driveMotor3(new COREMotor(DRIVEMOTOR3)), steerMotor3(new COREMotor(STEERMOTOR3)),
                       driveMotor4(new COREMotor(DRIVEMOTOR4)), steerMotor4(new COREMotor(STEERMOTOR4)){
        module1 = make_shared<CORESwerve::SwerveModule>(driveMotor1, steerMotor1, steerMotor1);
        module2 = make_shared<CORESwerve::SwerveModule>(driveMotor2, steerMotor2, steerMotor2);
        module3 = make_shared<CORESwerve::SwerveModule>(driveMotor3, steerMotor3, steerMotor3);
        module4 = make_shared<CORESwerve::SwerveModule>(driveMotor4, steerMotor4, steerMotor4);
        swerve = new CORESwerve (1.0, 1.0, module1, module2, module3, module4);
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
        swerve->cartesian(1, 0, 0);
        //swerve->run(0,0,0);
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