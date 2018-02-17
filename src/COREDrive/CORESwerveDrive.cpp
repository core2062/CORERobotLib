#include "CORESwerveDrive.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"

using namespace CORE;

CORESwerve::SwerveModule::SwerveModule(int driveMotorPortNumber, int steerMotorPortNumber, double angleOffset) :
        m_speedPIDController(0, 0, 0),
        m_anglePIDController(0, 0, 0),
        m_angleOffset(angleOffset),
		m_driveMotor(driveMotorPortNumber),
		m_steerMotor(steerMotorPortNumber) {

}


double CORESwerve::SwerveModule::getAngle(bool raw) {
    //Multiplying by 360 degrees and dividing by five volts
	//GetSensorCollection replaced getAnalogInRaw
    double angle = m_steerMotor.GetSensorCollection().GetAnalogIn() * (360.0 / 1025.0);
    if(raw) {
        return angle;
    } else {
        return angle - m_angleOffset;
    }

}

void CORESwerve::SwerveModule::setAnglePID(double p, double i, double d) {
    m_anglePIDController.setProportionalConstant(p);
    m_anglePIDController.setIntegralConstant(i);
    m_anglePIDController.setDerivativeConstant(d);
}

void CORESwerve::SwerveModule::setAngleOffset(double angleOffset) {
    m_angleOffset = angleOffset;
}

void CORESwerve::SwerveModule::zeroAngleOffset() {
    m_angleOffset = -getAngle(true);
}

void CORESwerve::SwerveModule::init() {
	m_steerMotor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::Analog, 0, 0);
	m_driveMotor.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);
	m_steerMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);
	m_driveMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);
	m_steerMotor.Set(ControlMode::PercentOutput, 0);
	m_driveMotor.Set(ControlMode::PercentOutput, 0);

}

string CORESwerve::SwerveModule::print() {
    string text = "\n\tSteer Motor Speed: " + to_string(m_steerMotor.GetSelectedSensorVelocity(0)); //TODO Change 0 to some other value
    text += "\n\tSteer Angle Offset: " + to_string(m_angleOffset);
    text += "\n\tSteer PID:";
    text += "\n\t\tkP: " + to_string(m_anglePIDController.getProportionalConstant());
    text += " kI: " + to_string(m_anglePIDController.getIntegralConstant());
    text += " kD: " + to_string(m_anglePIDController.getDerivativeConstant());
    text += " kF: " + to_string(m_anglePIDController.getFeedForwardConstant());
    text += "\n\t\tMistake: " + to_string(m_anglePIDController.getMistake());
    return text;
}

void CORESwerve::SwerveModule::resetEncoders() {
	m_steerMotor.GetSensorCollection().SetQuadraturePosition(0, 10);
	m_driveMotor.GetSensorCollection().SetQuadraturePosition(0, 10);
}

void CORESwerve::SwerveModule::resetMotors() {
	m_driveMotor.Set(ControlMode::PercentOutput, 0);
	m_steerMotor.Set(ControlMode::PercentOutput, 0);
}

void CORESwerve::SwerveModule::drive(double magnitude, double direction, double dt) {
    double steerMotorOutput;
    if(dt == -1) {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()),
                                         COREVector::FromCompassDegrees(direction));
    } else {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()),
                                                         COREVector::FromCompassDegrees(direction), dt);
    }
    std::cout<<"Steer Motor Output: "<<steerMotorOutput<<"; Magnitude: "<<magnitude<<std::endl;
    m_steerMotor.Set(ControlMode::PercentOutput, steerMotorOutput);
    m_driveMotor.Set(ControlMode::PercentOutput, magnitude);
    std::cout<<"Steer Motor Output: "<<steerMotorOutput<<"; Magnitude: "<<magnitude<<std::endl;

}

CORESwerve::CORESwerve(double trackWidth, double wheelBase,
		int leftFrontDriveMotorPort, int leftFrontSteerMotorPort, double leftFrontAngleOffset,
		int leftBackDriveMotorPort, int leftBackSteerMotorPort, double leftBackAngleOffset,
		int rightFrontDriveMotorPort, int rightFrontSteerMotorPort, double rightFrontAngleOffset,
		int rightBackDriveMotorPort, int rightBackSteerMotorPort, double rightBackAngleOffset) :
        m_leftFrontModule(leftFrontDriveMotorPort, leftFrontSteerMotorPort, leftFrontAngleOffset),
        m_leftBackModule(leftBackDriveMotorPort, leftBackSteerMotorPort, leftBackAngleOffset),
        m_rightBackModule(rightFrontDriveMotorPort, rightFrontSteerMotorPort, rightFrontAngleOffset),
        m_rightFrontModule(rightBackDriveMotorPort, rightBackSteerMotorPort, rightBackAngleOffset) {
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
}

void CORESwerve::init() {
	m_leftFrontModule.init();
	m_leftBackModule.init();
	m_rightFrontModule.init();
	m_rightBackModule.init();
}

void CORESwerve::setSteerPID(double kp, double ki, double kd) {
    m_leftFrontModule.setAnglePID(kp, ki, kd);
    m_rightFrontModule.setAnglePID(kp, ki, kd);
    m_rightBackModule.setAnglePID(kp, ki, kd);
    m_leftBackModule.setAnglePID(kp, ki, kd);
}

void CORESwerve::zeroAngleOffsets() {
    m_leftFrontModule.zeroAngleOffset();
    m_rightFrontModule.zeroAngleOffset();
    m_rightBackModule.zeroAngleOffset();
    m_leftBackModule.zeroAngleOffset();
}

void CORESwerve::setAngleOffsets(double leftFrontOffset, double leftBackOffset, double rightFrontOffset, double rightBackOffset) {
	m_leftFrontModule.setAngleOffset(leftFrontOffset);
	m_leftBackModule.setAngleOffset(leftBackOffset);
	m_rightFrontModule.setAngleOffset(rightFrontOffset);
	m_rightBackModule.setAngleOffset(rightBackOffset);
}

void CORESwerve::resetEncoders() {
	m_leftFrontModule.resetEncoders();
	m_leftBackModule.resetEncoders();
	m_rightFrontModule.resetEncoders();
	m_rightBackModule.resetEncoders();
}

void CORESwerve::resetMotors() {
	m_leftFrontModule.resetMotors();
	m_leftBackModule.resetMotors();
	m_rightFrontModule.resetMotors();
	m_rightBackModule.resetMotors();
}

double CORE::CORESwerve::getLeftFrontModuleAngle(bool raw) {
	return m_leftFrontModule.getAngle(raw);
}

double CORE::CORESwerve::getLeftBackModuleAngle(bool raw) {
	return m_leftBackModule.getAngle(raw);
}

double CORE::CORESwerve::getRightFrontModuleAngle(bool raw) {
	return m_rightFrontModule.getAngle(raw);
}

double CORE::CORESwerve::getRightBackModuleAngle(bool raw) {
	return m_rightBackModule.getAngle(raw);
}
void CORESwerve::calculate(double forward, double strafeRight, double rotateClockwise) {
	std::cout<<"Forward value: "<<forward<<std::endl<<"Strafe right value: "<<strafeRight<<"Rotate clockwise value: "<<rotateClockwise;
    if(forward == 0 && strafeRight == 0 && rotateClockwise == 0) {
        rightFrontModuleSpeed = 0;
        leftFrontModuleSpeed = 0;
        leftBackModuleSpeed = 0;
        rightBackModuleSpeed = 0;
        return;
    }
    double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));
    double a = strafeRight - rotateClockwise * (m_wheelbase / r);
    double b = strafeRight + rotateClockwise * (m_wheelbase / r);
    double c = forward - rotateClockwise * (m_trackwidth / r);
    double d = forward + rotateClockwise * (m_trackwidth / r);

    rightFrontModuleSpeed = sqrt(pow(b, 2) + pow(c, 2));
    leftFrontModuleSpeed = sqrt(pow(b, 2) + pow(d, 2));
    leftBackModuleSpeed = sqrt(pow(a, 2) + pow(d, 2));
    rightBackModuleSpeed = sqrt(pow(a, 2) + pow(c, 2));

    rightFrontModuleAngle = arctan(b, c);
    leftFrontModuleAngle = arctan(b, d);
    leftBackModuleAngle = arctan(a, d);
    rightBackModuleAngle = arctan(a, c);

    double maxSpeed = rightFrontModuleSpeed;

    if (leftFrontModuleSpeed > maxSpeed) {
        maxSpeed = leftFrontModuleSpeed;
    }
    if (leftBackModuleSpeed > maxSpeed) {
        maxSpeed = leftBackModuleSpeed;
    }
    if (rightBackModuleSpeed > maxSpeed) {
        maxSpeed = rightBackModuleSpeed;
    }
    if (maxSpeed > 1 && maxSpeed != 0) {
        rightFrontModuleSpeed /= maxSpeed;
        leftFrontModuleSpeed /= maxSpeed;
        leftBackModuleSpeed /= maxSpeed;
        rightBackModuleSpeed /= maxSpeed;
    }

    /*Code to determine if the motors should be set to be in reverse and rotate the wheels accordingly
    First checks to see if the angle from the driver would require turning more than 90 degrees
    If it does, set the angle to be the angle plus another 180 degrees, then take the remainder to
    make sure that the wheels don't make multiple rotations. Sets speed to negative*/

    double MAX_WHEEL_INVERT_SPEED = 1;

    if (abs(rightFrontModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) { 
        if (fabs(rightFrontModuleAngle - m_rightFrontModule.getAngle()) > 90 &&
                fabs(rightFrontModuleAngle - m_rightFrontModule.getAngle()) < 270) {
            rightFrontModuleAngle = fmod((rightFrontModuleAngle + 180), 360);
            rightFrontModuleSpeed = -rightFrontModuleSpeed;
        }
    }

    if (abs(leftFrontModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) { 
        if (fabs(leftFrontModuleAngle - m_leftFrontModule.getAngle()) > 90 &&
                fabs(leftFrontModuleAngle - m_leftFrontModule.getAngle()) < 270) {
            leftFrontModuleAngle = fmod((leftFrontModuleAngle + 180), 360);
            leftFrontModuleSpeed = -leftFrontModuleSpeed;
        }
    }

    if (abs(leftBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED){
        if (fabs(leftBackModuleAngle - m_leftBackModule.getAngle()) > 90 &&
                fabs(leftBackModuleAngle - m_leftBackModule.getAngle()) < 270) {
            leftBackModuleAngle = fmod((leftBackModuleAngle + 180), 360);
            leftBackModuleSpeed = -leftBackModuleSpeed;
        }
    }

    if (abs(rightBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) {
        if (fabs(rightBackModuleAngle - m_rightBackModule.getAngle()) > 90 &&
                fabs(rightBackModuleAngle - m_rightBackModule.getAngle()) < 270) {
            rightBackModuleAngle = fmod((rightBackModuleAngle + 180), 360);
            rightBackModuleSpeed = -rightBackModuleSpeed;
        }
    }
}

string CORESwerve::print() {
    string text = "Swerve Drive Status:";
    text += "\nFront Right Module\n\tRequested Angle: " + to_string(rightFrontModuleAngle) + "\tActual: "
            + to_string(m_rightFrontModule.getAngle()) + "\n\tRequested Speed: " + to_string(rightFrontModuleSpeed);
    text += m_rightFrontModule.print();
    text += "\nFront Left Module\n\tRequested Angle: " + to_string(leftFrontModuleAngle) + "\tActual: "
            + to_string(m_leftFrontModule.getAngle()) + "\n\tRequested Speed: " + to_string(leftFrontModuleSpeed);
    text += m_leftFrontModule.print();
    text += "\nBack Right Module\n\tRequested Angle: " + to_string(rightBackModuleAngle) + "\tActual: "
            + to_string(m_rightBackModule.getAngle()) + "\n\tRequested Speed: " + to_string(rightBackModuleSpeed);
    text += m_rightBackModule.print();
    text += "\nBack Left Module\n\tRequested Angle: " + to_string(leftBackModuleAngle) + "\tActual: "
            + to_string(m_leftBackModule.getAngle()) + "\n\tRequested Speed: " + to_string(leftBackModuleSpeed);
    text += m_leftBackModule.print();
    return text;
}

void CORESwerve::setMotors(double x, double y, double theta, double gyroYaw) {
	double forward = y * cos(gyroYaw) + x * sin(gyroYaw);
	double strafeRight = -y * sin(gyroYaw) + x * cos(gyroYaw);
	calculate(forward, strafeRight, theta);
	update();
	//std::cout<<print()<<std::endl;
}

void CORESwerve::haltMotors() {
	calculate(0, 0, 0);
	update();
}

void CORESwerve::update(double dt){
    m_leftFrontModule.drive(leftFrontModuleSpeed, leftFrontModuleAngle, dt);
    m_rightFrontModule.drive(rightFrontModuleSpeed, rightFrontModuleAngle, dt);
    m_rightBackModule.drive(rightBackModuleSpeed, rightBackModuleAngle, dt);
    m_leftBackModule.drive(leftBackModuleSpeed, leftBackModuleAngle, dt);
}

