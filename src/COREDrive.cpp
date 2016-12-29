#include "COREDrive.h"

using namespace CORE;

SwerveDrive::SwerveDrive(vector<shared_ptr<SwerveModule>> swerves){

}

void SwerveDrive::robotInit(){
	m_chooser.AddDefault("Mecanum", new std::string("mecanum"));
	m_chooser.AddObject("Throttle Mecanum", new std::string("mecanumthrottle"));
	m_chooser.AddObject("Field Based Mecanum", new std::string("fieldmecanum"));
	m_chooser.AddObject("Field Based Throttle Mecanum", new std::string("fieldmecanumthrottle"));
	m_chooser.AddObject("Smart Mecanum", new std::string("smartmecanum"));
	m_chooser.AddObject("Smart Field Based Mecanum", new std::string("smartfieldmecanum"));
	m_chooser.AddObject("Test", new std::string("test"));
	SmartDashboard::PutData("swerve-chooser", &m_chooser);

	if(m_modules.size() == 4){
		m_modules[0]->position = {-m_trackwidth*.5,m_wheelbase*.5};
		m_modules[1]->position = {m_trackwidth*.5,m_wheelbase*.5};
		m_modules[2]->position = {m_trackwidth*.5,-m_wheelbase*.5};
		m_modules[3]->position = {-m_trackwidth*.5,-m_wheelbase*.5};
	}
}

void SwerveDrive::setMode(ControlMode m){
	m_mode = m;
	m_forceMode = true;
}

void SwerveDrive::releaseMode(){
	m_forceMode = false;
	checkSD();
}

void SwerveDrive::checkSD(){
	std::string type = *(std::string*) m_chooser.GetSelected();
	if(type == "mecanum")
		m_mode = MECANUM;
	else if(type == "mecanumthrottle")
		m_mode = MECANUMTHROTTLE;
	else if(type == "fieldmecanum")
		m_mode = FIELDMECANUM;
	else if(type == "fieldmecanumthrottle")
		m_mode = FIELDMECANUMTHROTTLE;
	else if(type == "smartmecanum")
		m_mode = SMARTMECANUM;
	else if(type == "smartfieldmecanum")
		m_mode = SMARTFIELDMECANUM;
	else if (type == "test")
		m_mode = TEST;
}

void SwerveDrive::run(double y, double x, double r){
	if (!m_forceMode)
		checkSD();

	m_throttle = 1.0;
	m_x = 0.0;
	m_y = 0.0;
	m_rot = 0.0;

	switch(m_mode){
	case MECANUM:
		doMecanum();
		break;
	case MECANUMTHROTTLE:
		doMecanumThrottle();
		break;
	case FIELDMECANUM:
		doFieldMecanum();
		break;
	case FIELDMECANUMTHROTTLE:
		doFieldMecanumThrottle();
		break;
	case SMARTMECANUM:
		doSmartMecanum();
		break;
	case SMARTFIELDMECANUM:
		doSmartFieldMecanum();
		break;
	case TEST:
		doTest();
		break;
	}

	if(m_mode == SMARTMECANUM || m_mode == SMARTFIELDMECANUM){
		calculateSmart();
	}else{
		calculateNormal();
	}

	setMotors();
}

void SwerveDrive::setMotors(){
	//set motors
	for(auto i : m_modules){
		i->rotation->setPos(i->getSetValue(i->setAngle));
		i->rotation->calculate(0);
		i->drive->Set(i->setMagnitude * i->setDirection * m_throttle);
	}
}

void SwerveDrive::calculateNormal(){
	for(auto i : m_modules){
		i->setDirection = 1;
		double a,b;
		a = m_x + m_rot * i->position.unit().y;
		b = m_y - m_rot * i->position.unit().x;
		i->setMagnitude = sqrt(pow(a, 2) + pow(b, 2));
		double setAngle = (180.0/3.1415)*atan2(a, b);
		if (abs(setAngle - i->getCurrentAngle()) > 180){
			setAngle = i->clamp(setAngle - 180);
			i->setDirection = -1;
		}
		i->setAngle = setAngle;
	}
}

void SwerveDrive::calculateSmart(){
	for(auto i : m_modules){
		double a,b;
		a = m_x + m_rot * i->position.unit().y;
		b = m_y - m_rot * i->position.unit().x;
		i->setMagnitude = sqrt(pow(a,2) + pow(b,2));
		i->setAngle = (180.0/3.1415)*atan2(a,b);
	}
}

void SwerveDrive::doMecanum(){
	m_x = Robot::joystick(0)->getAxis(m_xAxis);
	m_y = Robot::joystick(0)->getAxis(m_yAxis);
	m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void SwerveDrive::doMecanumThrottle(){
	m_throttle = Robot::joystick(0)->getAxis(m_throttleAxis);
	m_x = Robot::joystick(0)->getAxis(m_xAxis);
	m_y = Robot::joystick(0)->getAxis(m_yAxis);
	m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void SwerveDrive::doFieldMecanum(){
	m_x = Robot::joystick(0)->getAxis(m_xAxis);
	m_y = Robot::joystick(0)->getAxis(m_yAxis);
	m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void SwerveDrive::doFieldMecanumThrottle(){
	m_throttle = Robot::joystick(0)->getAxis(m_throttleAxis);
	m_x = Robot::joystick(0)->getAxis(m_xAxis);
	m_y = Robot::joystick(0)->getAxis(m_yAxis);
	m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void SwerveDrive::doSmartMecanum(){

}

void SwerveDrive::doSmartFieldMecanum(){

}

void SwerveDrive::doTest(){

}
