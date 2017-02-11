#pragma once

#include <iostream>
#include "CORERobotLib.h"
#include "src/COREController/COREMotionProfile.h"
#include "WPILib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
    DRIVE_FL = 10,
    DRIVE_BL = 13,
    DRIVE_BR = 12,
    DRIVE_FR = 11
};

class DriveSubsystem : public CORESubsystem {
public:
    COREMotor driveMotorFL;
    COREMotor driveMotorBL;
    COREMotor driveMotorBR;
    COREMotor driveMotorFR;
    COREJoystick joystick1;
    COREConstant<string> testing;
    DriveSubsystem() : CORESubsystem("Drive Subsystem"),
                       driveMotorFL(DRIVE_FL), driveMotorBL(DRIVE_BL), driveMotorBR(DRIVE_BR), driveMotorFR(DRIVE_FR),
                       joystick1(0), testing("Test", "Test Value") {
        driveMotorBR.setReversed();
        driveMotorFR.setReversed();
    }

    inline void robotInit() {
    }

    inline void teleopInit() {
        driveMotorFR.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
        driveMotorFL.CANTalonController->SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    }

    inline void teleop() {
    	COREHardwareManager::zeroMotors();
    }

    inline void test() {
        cout << "Tested!" << endl;
    }
};


class driveDistance : public COREAutonAction {
public:
    COREMotor driveMotorFL;
    COREMotor driveMotorBL;
    COREMotor driveMotorBR;
    COREMotor driveMotorFR;
    driveDistance() :
            driveMotorFL(DRIVE_FL), driveMotorBL(DRIVE_BL), driveMotorBR(DRIVE_BR), driveMotorFR(DRIVE_FR) {
        driveMotorBR.setReversed();
        driveMotorFR.setReversed();
    }

    inline void actionInit() {
        driveMotorFR.CANTalonController->SetEncPosition(0);
    }

    inline actionStatus action() {
        if(abs(driveMotorFR.CANTalonController->GetEncPosition()) < 10000) {
            driveMotorFR.Set(0.25);
            driveMotorFL.Set(0.25);
            driveMotorBR.Set(0.25);
            driveMotorBL.Set(0.25);
            return actionStatus::CONTINUE;
        } else {
        	driveMotorFR.Set(0);
        	driveMotorFL.Set(0);
        	driveMotorBR.Set(0);
        	driveMotorBL.Set(0);
            return actionStatus::END;
        }
    }
};

class driveForwardAuto : public COREAuton {
public:
    driveForwardAuto() : COREAuton("Drive Forward", &m_driveForward, true) , m_driveForward(new driveDistance()) {

    }

    inline void addNodes() {
        //addFirstNode(&m_driveForward);
    }

private:
    Node m_driveForward;
};

//Directed Acyclic Graph


class Robot : public CORERobot {
public:
    //static shared_ptr<DriveSubsystem> driveSubsystem;
    driveForwardAuto auto1;
    DoubleSolenoid leftShifter;
    DoubleSolenoid rightShifter;
    COREJoystick joystick;
    COREMotor motor1;
    COREMotionProfile profile;
    bool highGear = false;
    Robot():
            //driveSubsystem(),
            auto1(),
			leftShifter(0, 1),
			rightShifter(2, 3),
			joystick(0),
            motor1(14),
			profile(1, 1)
    {

    }

    inline void robotInit() {
    	joystick.registerButton(COREJoystick::LEFT_BUTTON);
    	leftShifter.Set(DoubleSolenoid::kForward);
    	rightShifter.Set(DoubleSolenoid::kForward);
    }

    inline void teleopInit() {

    }

    inline void teleop() {
    	if(joystick.getRisingEdge(COREJoystick::LEFT_BUTTON)) {
    		if(highGear) {
    			leftShifter.Set(DoubleSolenoid::kReverse);
    			rightShifter.Set(DoubleSolenoid::kReverse);
    			highGear = false;
    		}
    		else {
    			leftShifter.Set(DoubleSolenoid::kForward);
    			rightShifter.Set(DoubleSolenoid::kForward);
    			highGear = true;
    		}
    	}
    	if(joystick.getRisingEdge(COREJoystick::A_BUTTON)) {
    	    profile.Set(100);
    	}
    }
};
