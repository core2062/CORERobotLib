#pragma once

#include <iostream>
#include "CORERobotLib.h"
#include "src/COREController/COREMotionProfile.h"
#include "src/WaypointFollower/WaypointFollower.h"
#include "src/COREDrive/COREEtherDrive.h"
#include "WPILib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
    DRIVE_FL = 10,
    DRIVE_BL = 13,
    DRIVE_BR = 12,
    DRIVE_FR = 11
};


class offSeasonRobot : public CORERobot {
public:
	CANTalon leftMaster, rightMaster, rightSlave, leftSlave;
    AHRS *gyro;

    DoubleSolenoid leftShift, rightShift;

    Joystick stick;

    TankTracker tracker;

    AdaptivePursuit pursue;

    bool following = false;

    offSeasonRobot():
    	leftMaster(10),
		rightMaster(11),
		rightSlave(12),
		leftSlave(13),
		leftShift(0,1),
		rightShift(2,3),
		stick(0),
		tracker(&leftMaster, &rightMaster, gyro),
		pursue(0.0, 0.0, .01, Path({{{0,0},0}}), false, 0.0)


    {
    	SmartDashboard::PutNumber("Drive P", 1.0);
    	SmartDashboard::PutNumber("Drive I", 0.0);
    	SmartDashboard::PutNumber("Drive D", 6.0);
    	SmartDashboard::PutNumber("Drive F", .5);
    	SmartDashboard::PutNumber("Drive Scrub", .15);

    	leftMaster.SetStatusFrameRateMs(CANTalon::StatusFrameRateFeedback, 10);
    	rightMaster.SetStatusFrameRateMs(CANTalon::StatusFrameRateFeedback, 10);

    	leftMaster.SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
    	leftMaster.Set(0);
    	leftSlave.SetTalonControlMode(CANTalon::TalonControlMode::kFollowerMode);
    	leftSlave.Set(10);
    	rightMaster.SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
 	   	rightMaster.Set(0);
    	rightSlave.SetTalonControlMode(CANTalon::TalonControlMode::kFollowerMode);
    	rightSlave.Set(11);

    	leftMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	leftMaster.SetSensorDirection(false);
    	leftMaster.ConfigEncoderCodesPerRev(1024);
    	leftMaster.SetInverted(false);
    	leftSlave.SetInverted(false);


    	rightMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Relative);
    	rightMaster.SetSensorDirection(true);
    	rightMaster.ConfigEncoderCodesPerRev(1024);
    	rightMaster.SetInverted(true);
    	rightSlave.SetInverted(false);


    	leftMaster.SetPID(1.0, 0.0, 6.0, .5);
    	rightMaster.SetPID(1.0, 0.0, 6.0, .5);



		try{
			gyro = new AHRS(SerialPort::Port::kMXP);
			std::cout << "Gyro maybe" << std::endl;
			std::cout << "Gyro Connected: " << gyro->IsConnected() << std::endl;
			std::cout << "Gyro Calibrate: " << gyro->IsCalibrating() << std::endl;
		} catch (std::exception& ex){
			std::cout << "Error: navX fail" << std::endl;
		}

		tracker = TankTracker(&leftMaster, &rightMaster, gyro);

		COREEtherDrive::setAB(.25,.75);
		COREEtherDrive::setQuickturn(1.5);
    }

    void robotInit() {

    }

    void teleopInit() {
    	gyro->ZeroYaw();
    	std::cout << "Tracker Reset" << std::endl;
    	tracker.reset(Timer::GetFPGATimestamp(), Position2d(Translation2d(),Rotation2d()));
    	std::cout << "Tracker Start" << std::endl;
    	tracker.start();
    	following = false;
    	configureNormalMode();
    }

    void teleop() {
        tracker.loop();
    	SmartDashboard::PutNumber("Left Pos", leftMaster.GetPosition());
    	SmartDashboard::PutNumber("Left RPS", leftMaster.GetSpeed() / 60.0);
    	SmartDashboard::PutNumber("Right Pos", rightMaster.GetPosition());
    	SmartDashboard::PutNumber("Right RPS", rightMaster.GetSpeed() / 60.0);
        if(following){
        	updatePathFollower();
        	if (pursue.isDone()){
        		following = false;
        		configureNormalMode();
        	}
        } else {
        	Position2d pos = tracker.getLatestFieldToVehicle();
        	Translation2d xy = pos.getTranslation();
        	Rotation2d t = pos.getRotation();
        	SmartDashboard::PutNumber("Robot X", xy.getX());
        	SmartDashboard::PutNumber("Robot Y", xy.getY());
        	SmartDashboard::PutNumber("Robot Theta", t.getDegrees());

        	if (stick.GetRawButton(1)){
        		std::cout << "Starting" << std::endl;
        		tracker.reset(Timer::GetFPGATimestamp(), Position2d());
        		Path newPath({
        			{{0,0},102},
					{{25,13},102},
					{{55,36}, 102},
					{{60,36}, 102, "Test"},
					{{65,36}, 102},
					{{95,13}, 102},
					{{120, 0}, 102}
					//{{500,0},72}
        		});
        		followPath(newPath, false);
        		return;
        	}

        	double mag = -stick.GetRawAxis(1);
        	double rot = stick.GetRawAxis(2);

        	VelocityPair ether = COREEtherDrive::calculate(mag, rot, .1);

        	leftMaster.Set(ether.left);
        	rightMaster.Set(ether.right);

			if(stick.GetRawButton(5)){
				leftShift.Set(DoubleSolenoid::kForward);
				rightShift.Set(DoubleSolenoid::kForward);
			}

			if(stick.GetRawButton(6)){
				leftShift.Set(DoubleSolenoid::kReverse);
				rightShift.Set(DoubleSolenoid::kReverse);
			}



        }
    }

    void configureSpeedMode(){
    	leftMaster.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
    	leftMaster.SetPID(SmartDashboard::GetNumber("Drive P", 1.0), SmartDashboard::GetNumber("Drive I", 0.0),
    			SmartDashboard::GetNumber("Drive D", 0.0), SmartDashboard::GetNumber("Drive F", 0.0));
    	rightMaster.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
    	rightMaster.SetPID(SmartDashboard::GetNumber("Drive P", 1.0), SmartDashboard::GetNumber("Drive I", 0.0),
    			SmartDashboard::GetNumber("Drive D", 0.0), SmartDashboard::GetNumber("Drive F", 0.0));
    }

    void configureNormalMode(){
    	leftMaster.SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
    	rightMaster.SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
    }

    void followPath(Path path, bool reversed){
    	if(!following){
    		//configureSpeedMode();
    		configureNormalMode();
    		following = true;
    	}
    	gyro->ZeroYaw();
    	std::cout << "Tracker Reset" << std::endl;
    	tracker.reset(Timer::GetFPGATimestamp(), Position2d());
    	std::cout << "Tracker Start" << std::endl;
    	tracker.start();
    	pursue = AdaptivePursuit(24.0, 50.0, .01, path, reversed, .25);
    	updatePathFollower();
    }

    void updatePathFollower(){
    	Position2d pos = tracker.getLatestFieldToVehicle();
    	std::cout << std::endl;
//    	std::cout << "X: " << pos.getTranslation().getX() << std::endl;
    	Position2d::Delta command = pursue.update(pos, Timer::GetFPGATimestamp());
    	std::cout  << "Command: " << command.dx << "," << command.dy << "," << command.dtheta << std::endl;
    	VelocityPair setpoint = TankKinematics::inverseKinematics(command);

    	double maxVel = 0.0;
    	maxVel = max(maxVel, setpoint.left);
    	maxVel = max(maxVel, setpoint.right);
//    	std::cout << "Biggest Val: " << maxVel << std::endl;
    	if (maxVel > /*102*/102){
    		double scaling = /*102*/102 / maxVel;
    		setpoint = VelocityPair(setpoint.left * scaling, setpoint.right * scaling);
    	}
    	std::cout << "Setpoint: " << setpoint.left << "," << setpoint.right<< std::endl;

    	leftMaster.Set(setpoint.left / 102);
    	rightMaster.Set(setpoint.right / 102);

    	//leftMaster.Set(setpoint.left / ratio);
    	//rightMaster.Set(-1 * setpoint.right / ratio);


    }
};
