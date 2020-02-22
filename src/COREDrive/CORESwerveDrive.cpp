#include "CORESwerveDrive.h"

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
        CORELog::LogError("A module passed to CORESwerve is a nullptr!");
    }
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
    m_wheelCircumference = wheelDiameter * PI;
    m_ticksToRotation = ticksToRotation;

    m_frontLeftModule->SetAngleOffset(m_leftFrontModuleOffset.Get());
    m_frontRightModule->SetAngleOffset(m_rightFrontModuleOffset.Get());
    m_backLeftModule->SetAngleOffset(m_leftBackModuleOffset.Get());
    m_backRightModule->SetAngleOffset(m_rightBackModuleOffset.Get());
}

CORESwerve::SwerveModule::SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor) :
        m_speedPIDController(0, 0, 0),
        m_anglePIDController(0, 0, 0),
        m_driveMotor(driveMotor),
        m_steerMotor(steerMotor) {
}

double CORESwerve::SwerveModule::GetAngle(bool raw) {
    //Multiplying by 360 degrees and dividing by five volts
	//GetSensorCollection replaced getAnalogInRaw
    double angle = m_steerMotor->GetSensorCollection().GetAnalogInRaw() * (360.0 / 1024.0);
    if(!raw) {
        angle -= m_angleOffset;
    }
    return angle;
}

COREVector CORESwerve::SwerveModule::ForwardKinematics(double wheelCircumference, double ticksToRotation) {
	//Calculates the individual vector of each of the modules
	//Theta may be change in angle or the total angle
	double magnitude = ((GetEncoder() / ticksToRotation) * wheelCircumference);
    double newMag = magnitude - m_lastMagnitude;
	m_lastMagnitude = magnitude;
	return COREVector::FromCompassDegrees(GetAngle(), newMag);
}

void CORESwerve::SwerveModule::ZeroAngle() {
	m_angleOffset = -GetAngle(true);
}

void CORESwerve::SwerveModule::ZeroEncoder() {
    m_driveMotor->SetSelectedSensorPosition(0, 0, 0);
    m_lastMagnitude = 0;
}

double CORESwerve::SwerveModule::GetEncoder() {
	return -m_driveMotor->GetSelectedSensorPosition(0);
}

void CORESwerve::SwerveModule::SetAnglePID(double p, double i, double d) {
    m_anglePIDController.SetProportionalConstant(p);
    m_anglePIDController.SetIntegralConstant(i);
    m_anglePIDController.SetDerivativeConstant(d);
}

void CORESwerve::SwerveModule::Drive(COREVector vector, double dt) {
    double steerMotorOutput;
    if(dt == -1) {
        steerMotorOutput = m_anglePIDController.Calculate(COREVector::FromCompassDegrees(GetAngle()), vector);
    } else {
        steerMotorOutput = m_anglePIDController.Calculate(COREVector::FromCompassDegrees(GetAngle()), vector, dt);
    }
    m_steerMotor->Set(ControlMode::PercentOutput, steerMotorOutput);
    m_driveMotor->Set(ControlMode::PercentOutput, vector.GetMagnitude());
}

string CORESwerve::SwerveModule::Print() {
    string text = "\n\tSteer Motor Speed: " + to_string(m_steerMotor->GetSelectedSensorVelocity(0)); //TODO Change 0 to some other value
    text += "\n\tSteer Angle Offset: " + to_string(m_angleOffset);
    text += "\n\tSteer PID:";
    text += "\n\t\tkP: " + to_string(m_anglePIDController.GetProportionalConstant());
    text += " kI: " + to_string(m_anglePIDController.GetIntegralConstant());
    text += " kD: " + to_string(m_anglePIDController.GetDerivativeConstant());
    text += " kF: " + to_string(m_anglePIDController.GetFeedForwardConstant());
    text += "\n\t\tMistake: " + to_string(m_anglePIDController.GetMistake());
    return text;
}

void CORESwerve::SwerveModule::SetAngleOffset(double angleOffset) {
    m_angleOffset = angleOffset;
}

void CORESwerve::InverseKinematics(double x, double y, double theta) {
    if(y == 0 && x == 0 && theta == 0) {
        m_frontRight.SetMagnitude(0);
        m_frontLeft.SetMagnitude(0);
        m_backRight.SetMagnitude(0);
        m_backLeft.SetMagnitude(0);
        m_frontLeftModule->Drive(m_frontLeft, -1);
        m_frontRightModule->Drive(m_frontRight, -1);
        m_backRightModule->Drive(m_backRight, -1);
        m_backLeftModule->Drive(m_backLeft, -1);
        return;
    }

    double a = x - theta * (m_wheelbase / 2.0);
    double b = x + theta * (m_wheelbase / 2.0);
    double c = y - theta * (m_trackwidth / 2.0);
    double d = y + theta * (m_trackwidth / 2.0);

    double frontRightModuleSpeed = sqrt(pow(b, 2) + pow(c, 2));
    double frontLeftModuleSpeed = sqrt(pow(b, 2) + pow(d, 2));
    double backLeftModuleSpeed = sqrt(pow(a, 2) + pow(d, 2));
    double backRightModuleSpeed = sqrt(pow(a, 2) + pow(c, 2));

    double frontRightModuleAngle = Arctan(b, c);
    double frontLeftModuleAngle = Arctan(b, d);
    double backLeftModuleAngle = Arctan(a, d);
    double backRightModuleAngle = Arctan(a, c);

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
#define OPTIMIZE
#ifdef OPTIMIZE
    double MAX_WHEEL_INVERT_SPEED = 1;

    if(abs(m_frontRight.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
        && abs(COREVector::FromCompassDegrees(m_frontRightModule->GetAngle()).ShortestRotationTo(m_frontRight)) > 90) {
        m_frontRight.Invert();
    }

    if(abs(m_frontLeft.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
        && abs(COREVector::FromCompassDegrees(m_frontLeftModule->GetAngle()).ShortestRotationTo(m_frontLeft)) > 90) {
        m_frontLeft.Invert();
    }

    if(abs(m_backRight.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
       && abs(COREVector::FromCompassDegrees(m_backRightModule->GetAngle()).ShortestRotationTo(m_backRight)) > 90) {
        m_backRight.Invert();
    }

    if(abs(m_backLeft.GetMagnitude()) <= MAX_WHEEL_INVERT_SPEED
       && abs(COREVector::FromCompassDegrees(m_backLeftModule->GetAngle()).ShortestRotationTo(m_backLeft)) > 90) {
        m_backLeft.Invert();
    }
#endif

    m_frontLeftModule->Drive(m_frontLeft, -1);
    m_frontRightModule->Drive(m_frontRight, -1);
    m_backRightModule->Drive(m_backRight, -1);
    m_backLeftModule->Drive(m_backLeft, -1);
}

SwervePosition2d CORESwerve::ForwardKinematics() {
    
	//Adds the up all of the vector of each of the modules to get a total vector

	COREVector frontLeft = m_frontLeftModule->ForwardKinematics(m_wheelCircumference, m_ticksToRotation);
    frc::SmartDashboard::PutNumber("Front Left X: ", frontLeft.GetX());
    frc::SmartDashboard::PutNumber("Front Left Y: ", frontLeft.GetY());
    frc::SmartDashboard::PutNumber("Front Left Encoder: ", m_frontLeftModule->GetEncoder());

	COREVector frontRight = m_frontRightModule->ForwardKinematics(m_wheelCircumference, m_ticksToRotation);
    frc::SmartDashboard::PutNumber("Front Right X: ", frontRight.GetX());
    frc::SmartDashboard::PutNumber("Front Right Y: ", frontRight.GetY());
    frc::SmartDashboard::PutNumber("Front Right Encoder: ", m_frontRightModule->GetEncoder());

	COREVector backLeft = m_backLeftModule->ForwardKinematics(m_wheelCircumference, m_ticksToRotation);
    frc::SmartDashboard::PutNumber("Back Left X: ", backLeft.GetX());
    frc::SmartDashboard::PutNumber("Back Left Y: ", backLeft.GetY());
    frc::SmartDashboard::PutNumber("Back Left Encoder: ", m_backLeftModule->GetEncoder());

	COREVector backRight = m_backRightModule->ForwardKinematics(m_wheelCircumference, m_ticksToRotation);
    frc::SmartDashboard::PutNumber("Back Right X: ", backRight.GetX());
    frc::SmartDashboard::PutNumber("Back Right Y: ", backRight.GetY());
    frc::SmartDashboard::PutNumber("Back Right Encoder: ", m_backRightModule->GetEncoder());

    double backX = (backLeft.GetX() + backRight.GetX()) / 2.0;
    double frontX = (frontLeft.GetX() + frontRight.GetX()) / 2.0;
    double rightY = (frontRight.GetY() + backRight.GetY()) / 2.0;
    double leftY = (frontLeft.GetY() + backLeft.GetY()) / 2.0;

    double xOmega = (frontX - backX) / m_wheelbase;
    double yOmega = (leftY - rightY) / m_trackwidth;
    double omega = (xOmega + yOmega) / 2.0;

    double sumX = ((frontLeft.GetX() + frontRight.GetX() + backLeft.GetX() + backRight.GetX()) / 4.0);
    double sumY = ((frontLeft.GetY() + frontRight.GetY() + backLeft.GetY() + backRight.GetY()) / 4.0);

    return SwervePosition2d({sumX, sumY}, SwerveRotation2d::FromRadians(omega));
}

void CORESwerve::SetSteerPID(double kp, double ki, double kd) {
    m_frontLeftModule->SetAnglePID(kp, ki, kd);
    m_frontRightModule->SetAnglePID(kp, ki, kd);
    m_backRightModule->SetAnglePID(kp, ki, kd);
    m_backLeftModule->SetAnglePID(kp, ki, kd);
}

void CORESwerve::ZeroOffsets() {
    m_leftFrontModuleOffset.Set(m_frontLeftModule->GetAngle(true));
    m_rightFrontModuleOffset.Set(m_frontRightModule->GetAngle(true));
    m_leftBackModuleOffset.Set(m_backLeftModule->GetAngle(true));
    m_rightBackModuleOffset.Set(m_backRightModule->GetAngle(true));

    UpdateOffsets();
}

void CORESwerve::UpdateOffsets() {
    m_frontLeftModule->SetAngleOffset(m_leftFrontModuleOffset.Get());
    m_frontRightModule->SetAngleOffset(m_rightFrontModuleOffset.Get());
    m_backLeftModule->SetAngleOffset(m_leftBackModuleOffset.Get());
    m_backRightModule->SetAngleOffset(m_rightBackModuleOffset.Get());
}

void CORESwerve::ZeroEncoders() {
    m_frontLeftModule->ZeroEncoder();
    m_frontRightModule->ZeroEncoder();
    m_backLeftModule->ZeroEncoder();
    m_backRightModule->ZeroEncoder();
}