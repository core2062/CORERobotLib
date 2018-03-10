#include "CORESwerveDrive.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase, double wheelDiameter,
					   double ticksToRotation,
                       SwerveModule *leftFrontModule,
                       SwerveModule *leftBackModule,
                       SwerveModule *rightBackModule,
                       SwerveModule *rightFrontModule) :
        m_frontLeftModule(leftFrontModule),
        m_backLeftModule(leftBackModule),
        m_backRightModule(rightBackModule),
        m_frontRightModule(rightFrontModule),
        m_leftFrontModuleOffset("Front Left Module Offset"),
        m_leftBackModuleOffset("Back Left Module Offset"),
        m_rightBackModuleOffset("Back Right Module Offset"),
        m_rightFrontModuleOffset("Front Right Module Offset") {
    if(!(m_frontLeftModule && m_backLeftModule && m_backRightModule && m_frontRightModule)) {
        CORELog::logError("A module passed to CORESwerve is a nullptr!");
    }
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
    m_wheelCircumference = wheelDiameter * PI;
    m_ticksToRotation = ticksToRotation;

    m_frontLeftModule->setAngleOffset(m_leftFrontModuleOffset.Get());
    m_frontRightModule->setAngleOffset(m_rightFrontModuleOffset.Get());
    m_backLeftModule->setAngleOffset(m_leftBackModuleOffset.Get());
    m_backRightModule->setAngleOffset(m_rightBackModuleOffset.Get());
}

CORESwerve::SwerveModule::SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor) :
        m_speedPIDController(0, 0, 0),
        m_anglePIDController(0, 0, 0),
        m_driveMotor(driveMotor),
        m_steerMotor(steerMotor) {
}


double CORESwerve::SwerveModule::getAngle(bool raw) {
    //Multiplying by 360 degrees and dividing by five volts
	//GetSensorCollection replaced getAnalogInRaw
    double angle = m_steerMotor->GetSensorCollection().GetAnalogInRaw() * (360.0 / 1025.0);
    if(raw) {
        return angle;
    } else {
        return angle - m_angleOffset;
    }

}

COREVector CORESwerve::SwerveModule::inverseKinematics(
		double wheelCircumference, double ticksToRotation) {
	//Calculates the individual vector of each of the modules
	//Theta may be change in angle or the total angle
	double magnitude = (getEncoder() * wheelCircumference) / ticksToRotation - m_lastMagnitude;
	m_lastMagnitude += magnitude;
	return COREVector::FromCompassDegrees(getAngle(), magnitude);
}

void CORESwerve::SwerveModule::zeroAngle() {
	m_angleOffset = -getAngle(true);
}

void CORESwerve::SwerveModule::zeroEncoder() {
    m_driveMotor->SetSelectedSensorPosition(0, 0, 0);
}

double CORESwerve::SwerveModule::getEncoder() {
	return -m_driveMotor->GetSelectedSensorPosition(0);
}
void CORESwerve::SwerveModule::setAnglePID(double p, double i, double d) {
    m_anglePIDController.setProportionalConstant(p);
    m_anglePIDController.setIntegralConstant(i);
    m_anglePIDController.setDerivativeConstant(d);
}

void CORESwerve::tank(double speed, double rot){
//    tank(COREEtherDrive::calculate(speed, rot, .1));
}


void CORESwerve::SwerveModule::drive(COREVector vector, double dt) {
    double steerMotorOutput;
    if(dt == -1) {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()), vector);
    } else {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()), vector, dt);
    }
    //TODO check percent output
    m_steerMotor->Set(ControlMode::PercentOutput, steerMotorOutput);
    m_driveMotor->Set(ControlMode::PercentOutput, vector.GetMagnitude());
}

string CORESwerve::SwerveModule::print() {
    string text = "\n\tSteer Motor Speed: " + to_string(m_steerMotor->GetSelectedSensorVelocity(0)); //TODO Change 0 to some other value
    text += "\n\tSteer Angle Offset: " + to_string(m_angleOffset);
    text += "\n\tSteer PID:";
    text += "\n\t\tkP: " + to_string(m_anglePIDController.getProportionalConstant());
    text += " kI: " + to_string(m_anglePIDController.getIntegralConstant());
    text += " kD: " + to_string(m_anglePIDController.getDerivativeConstant());
    text += " kF: " + to_string(m_anglePIDController.getFeedForwardConstant());
    text += "\n\t\tMistake: " + to_string(m_anglePIDController.getMistake());
    return text;
}

void CORESwerve::SwerveModule::setAngleOffset(double angleOffset) {
    m_angleOffset = angleOffset;
}

void CORESwerve::calculate(double x, double y, double theta) {
    if(y == 0 && x == 0 && theta == 0) {
        m_frontRight.SetMagnitude(0);
        m_frontLeft.SetMagnitude(0);
        m_backRight.SetMagnitude(0);
        m_backLeft.SetMagnitude(0);
        return;
    }
    double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));
    double a = x - theta * (m_wheelbase / r);
    double b = x + theta * (m_wheelbase / r);
    double c = y - theta * (m_trackwidth / r);
    double d = y + theta * (m_trackwidth / r);

    double frontRightModuleSpeed = sqrt(pow(b, 2) + pow(c, 2));
    double frontLeftModuleSpeed = sqrt(pow(b, 2) + pow(d, 2));
    double backLeftModuleSpeed = sqrt(pow(a, 2) + pow(d, 2));
    double backRightModuleSpeed = sqrt(pow(a, 2) + pow(c, 2));

    double frontRightModuleAngle = arctan(b, c);
    double frontLeftModuleAngle = arctan(b, d);
    double backLeftModuleAngle = arctan(a, d);
    double backRightModuleAngle = arctan(a, c);

    double maxSpeed = frontRightModuleSpeed;

    if (frontLeftModuleSpeed > maxSpeed) {
        maxSpeed = frontLeftModuleSpeed;
    }
    if (backLeftModuleSpeed > maxSpeed) {
        maxSpeed = backLeftModuleSpeed;
    }
    if (backRightModuleSpeed > maxSpeed) {
        maxSpeed = backRightModuleSpeed;
    }
    if (maxSpeed > 1 && maxSpeed != 0) {
        frontRightModuleSpeed /= maxSpeed;
        frontLeftModuleSpeed /= maxSpeed;
        backLeftModuleSpeed /= maxSpeed;
        backRightModuleSpeed /= maxSpeed;
    }

    m_frontRight = COREVector::FromCompassDegrees(frontRightModuleAngle, frontRightModuleSpeed);
    m_frontLeft = COREVector::FromCompassDegrees(frontLeftModuleAngle, frontLeftModuleSpeed);
    m_backRight = COREVector::FromCompassDegrees(backRightModuleAngle, backRightModuleSpeed);
    m_backLeft = COREVector::FromCompassDegrees(backLeftModuleAngle, backLeftModuleSpeed);


    /*Code to determine if the motors should be set to be in reverse and rotate the wheels accordingly
    First checks to see if the angle from the driver would require turning more than 90 degrees
    If it does, set the angle to be the angle plus another 180 degrees, then take the remainder to
    make sure that the wheels don't make multiple rotations. Sets speed to negative*/

    double MAX_WHEEL_INVERT_SPEED = 1;

    if(abs(m_frontRight.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
        && abs(COREVector::FromCompassDegrees(m_frontRightModule->getAngle()).ShortestRotationTo(m_frontRight)) > 90) {
        m_frontRight.Invert();
    }

    if(abs(m_frontLeft.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
        && abs(COREVector::FromCompassDegrees(m_frontLeftModule->getAngle()).ShortestRotationTo(m_frontLeft)) > 90) {
        m_frontLeft.Invert();
    }

    if(abs(m_backRight.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
       && abs(COREVector::FromCompassDegrees(m_backRightModule->getAngle()).ShortestRotationTo(m_backRight)) > 90) {
        m_backRight.Invert();
    }

    if(abs(m_backLeft.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
       && abs(COREVector::FromCompassDegrees(m_backLeftModule->getAngle()).ShortestRotationTo(m_backLeft)) > 90) {
        m_backLeft.Invert();
    }
}

COREVector CORESwerve::inverseKinematics(double gyroAngle) {
	//Adds the up all of the vector of each of the modules to get a total vector
	double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));

	COREVector leftFront = m_frontLeftModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	double leftFrontX = leftFront.GetX() + gyroAngle * (m_wheelbase / r);
	double leftFrontY = leftFront.GetY() + gyroAngle * (m_trackwidth / r);
	leftFront.SetXY(leftFrontX, leftFrontY);
	SmartDashboard::PutNumber("Left Front X: ", leftFront.GetX());
	SmartDashboard::PutNumber("Left Front Y: ", leftFront.GetY());
    SmartDashboard::PutNumber("Left Front Encoder: ", m_frontLeftModule->getEncoder());

	COREVector rightFront = m_frontRightModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	double rightFrontX = rightFront.GetX() + gyroAngle * (m_wheelbase / r);
	double rightFrontY = rightFront.GetY() - gyroAngle * (m_trackwidth / r);
	rightFront.SetXY(rightFrontX, rightFrontY);
	SmartDashboard::PutNumber("Right Front X: ", rightFront.GetX());
	SmartDashboard::PutNumber("Right Front Y: ", rightFront.GetY());
    SmartDashboard::PutNumber("Right Front Encoder: ", m_frontRightModule->getEncoder());

	COREVector leftBack = m_backLeftModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	double leftBackX = leftBack.GetX() - gyroAngle * (m_wheelbase / r);
	double leftBackY = leftBack.GetY() + gyroAngle * (m_wheelbase / r);
	leftBack.SetXY(leftBackX, leftBackY);
	SmartDashboard::PutNumber("Left Back X: ", leftBack.GetX());
	SmartDashboard::PutNumber("Left Back Y: ", leftBack.GetY());
    SmartDashboard::PutNumber("Left Back Encoder: ", m_backLeftModule->getEncoder());

	COREVector rightBack = m_backRightModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	double rightBackX = rightBack.GetX() - gyroAngle * (m_wheelbase / r);
	double rightBackY =  rightBack.GetY() - gyroAngle * (m_trackwidth / r);
	rightBack.SetXY(rightBackX, rightBackY);
	SmartDashboard::PutNumber("Right Back X: ", rightBack.GetX());
	SmartDashboard::PutNumber("Right Back Y: ", rightBack.GetY());
    SmartDashboard::PutNumber("Right Back Encoder: ", m_backRightModule->getEncoder());


    return leftFront.AddVector(rightFront.AddVector(leftBack.AddVector(rightBack)));

}

void CORESwerve::update(double dt){
    m_frontLeftModule->drive(m_frontLeft, dt);
    m_frontRightModule->drive(m_frontRight, dt);
    m_backRightModule->drive(m_backRight, dt);
    m_backLeftModule->drive(m_backLeft, dt);
}

/*string CORESwerve::print() {
    string text = "Swerve Drive Status:";
    text += "\nFront Right Module\n\tRequested Angle: " + to_string(rightFrontModuleAngle) + "\tActual: "
            + to_string(m_rightFrontModule->getAngle()) + "\n\tRequested Speed: " + to_string(rightFrontModuleSpeed);
    text += m_rightFrontModule->print();
    text += "\nFront Left Module\n\tRequested Angle: " + to_string(leftFrontModuleAngle) + "\tActual: "
            + to_string(m_leftFrontModule->getAngle()) + "\n\tRequested Speed: " + to_string(leftFrontModuleSpeed);
    text += m_leftFrontModule->print();
    text += "\nBack Right Module\n\tRequested Angle: " + to_string(rightBackModuleAngle) + "\tActual: "
            + to_string(m_rightBackModule->getAngle()) + "\n\tRequested Speed: " + to_string(rightBackModuleSpeed);
    text += m_rightBackModule->print();
    text += "\nBack Left Module\n\tRequested Angle: " + to_string(leftBackModuleAngle) + "\tActual: "
            + to_string(m_leftBackModule->getAngle()) + "\n\tRequested Speed: " + to_string(leftBackModuleSpeed);
    text += m_leftBackModule->print();
    return text;
}*/

void CORESwerve::setSteerPID(double kp, double ki, double kd) {
    m_frontLeftModule->setAnglePID(kp, ki, kd);
    m_frontRightModule->setAnglePID(kp, ki, kd);
    m_backRightModule->setAnglePID(kp, ki, kd);
    m_backLeftModule->setAnglePID(kp, ki, kd);
}

void CORESwerve::zeroOffsets() {
    m_leftFrontModuleOffset.Set(m_frontLeftModule->getAngle(true));
    m_frontLeftModule->setAngleOffset(m_leftFrontModuleOffset.Get());
    
    m_rightFrontModuleOffset.Set(m_frontRightModule->getAngle(true));
    m_frontRightModule->setAngleOffset(m_rightFrontModuleOffset.Get());

    m_leftBackModuleOffset.Set(m_backLeftModule->getAngle(true));
    m_backLeftModule->setAngleOffset(m_leftBackModuleOffset.Get());

    m_rightBackModuleOffset.Set(m_backRightModule->getAngle(true));
    m_backRightModule->setAngleOffset(m_rightBackModuleOffset.Get());
}
