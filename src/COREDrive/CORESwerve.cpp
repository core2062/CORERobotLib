#include "CORESwerve.h"

using namespace CORE;

/*    m_chooser.AddDefault("Mecanum", new std::string("mecanum"));
    m_chooser.AddObject("Throttle Mecanum", new std::string("mecanumthrottle"));
    m_chooser.AddObject("Field Based Mecanum", new std::string("fieldmecanum"));
    m_chooser.AddObject("Field Based Throttle Mecanum", new std::string("fieldmecanumthrottle"));
    m_chooser.AddObject("Smart Mecanum", new std::string("smartmecanum"));
    m_chooser.AddObject("Smart Field Based Mecanum", new std::string("smartfieldmecanum"));
    m_chooser.AddObject("Test", new std::string("test"));
    SmartDashboard::PutData("swerve-chooser", &m_chooser);*//*



    if(m_modules.size() == 4){
        m_modules[0]->m_position = {-m_trackwidth*.5,m_wheelbase*.5};
        m_modules[1]->m_position = {m_trackwidth*.5,m_wheelbase*.5};
        m_modules[2]->m_position = {m_trackwidth*.5,-m_wheelbase*.5};
        m_modules[3]->m_position = {-m_trackwidth*.5,-m_wheelbase*.5};
    }

void CORESwerve::setMode(ControlMode m){
    m_mode = m;
    m_forceMode = true;
}

void CORESwerve::releaseMode(){
    m_forceMode = false;
    checkSD();
}

void CORESwerve::checkSD(){
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

void CORESwerve::run(double y, double x, double r){
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

void CORESwerve::setMotors(){
    //set motors
    for(auto i : m_modules){
        i->m_steerMotor->setPos(i->getSetValue(i->m_setAngle));
        i->m_steerMotor->calculate(0);
        i->m_driveMotor->Set(i->m_setMagnitude * i->m_setDirection * m_throttle);
    }
}

void CORESwerve::calculateNormal(){
    for(auto i : m_modules){
        i->m_setDirection = 1;
        double a,b;
        a = m_x + m_rot * i->m_position.unit().y;
        b = m_y - m_rot * i->m_position.unit().x;
        i->m_setMagnitude = sqrt(pow(a, 2) + pow(b, 2));
        double setAngle = (180.0/3.1415)*atan2(a, b);
        if (abs(setAngle - i->getCurrentAngle()) > 180){
            setAngle = i->clamp(setAngle - 180);
            i->m_setDirection = -1;
        }
        i->m_setAngle = setAngle;
    }
}

void CORESwerve::calculateSmart(){
    for(auto i : m_modules){
        double a,b;
        a = m_x + m_rot * i->m_position.unit().y;
        b = m_y - m_rot * i->m_position.unit().x;
        i->m_setMagnitude = sqrt(pow(a,2) + pow(b,2));
        i->m_setAngle = (180.0/3.1415)*atan2(a,b);
    }
}

void CORESwerve::doMecanum(){
    m_x = Robot::joystick(0)->getAxis(m_xAxis);
    m_y = Robot::joystick(0)->getAxis(m_yAxis);
    m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void CORESwerve::doMecanumThrottle(){
    m_throttle = Robot::joystick(0)->getAxis(m_throttleAxis);
    m_x = Robot::joystick(0)->getAxis(m_xAxis);
    m_y = Robot::joystick(0)->getAxis(m_yAxis);
    m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void CORESwerve::doFieldMecanum(){
    m_x = Robot::joystick(0)->getAxis(m_xAxis);
    m_y = Robot::joystick(0)->getAxis(m_yAxis);
    m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}

void CORESwerve::doFieldMecanumThrottle(){
    m_throttle = Robot::joystick(0)->getAxis(m_throttleAxis);
    m_x = Robot::joystick(0)->getAxis(m_xAxis);
    m_y = Robot::joystick(0)->getAxis(m_yAxis);
    m_rot = Robot::joystick(0)->getAxis(m_rotXAxis);
}*/

/*--------------------------------------------------------------------------------------------------------------------*/


CORESwerve::CORESwerve(double wheelBase, double trackWidth,
                       const shared_ptr<CORESwerve::SwerveModule>& leftFrontModule,
                       const shared_ptr<CORESwerve::SwerveModule>& leftBackModule,
                       const shared_ptr<CORESwerve::SwerveModule>& rightBackModule,
                       const shared_ptr<CORESwerve::SwerveModule>& rightFrontModule) :
        m_leftFrontModule(leftFrontModule), m_leftBackModule(leftBackModule),
        m_rightBackModule(rightBackModule), m_rightFrontModule(rightFrontModule) {
    m_modules.push_back(leftFrontModule);
    m_modules.push_back(leftBackModule);
    m_modules.push_back(rightBackModule);

    double x = m_trackwidth*.5;
    double y = m_wheelbase*.5;
    m_leftFrontModule->x = -x;
    m_leftFrontModule->y = y;
    m_leftBackModule->x = -x;
    m_leftBackModule->y = -y;
    m_rightBackModule->x = x;
    m_rightBackModule->y = -y;
    if(m_rightFrontModule != nullptr) {
        m_modules.push_back(rightFrontModule);
        m_rightFrontModule->x = x;
        m_rightFrontModule->y = y;
    }
}

void CORESwerve::setRotation(double rotation) {

}

double CORESwerve::getRotation() {
    return 0;
}

void CORESwerve::cartesian(double x, double y, double rotation) {
    m_x = x;
    m_y = y;
    m_rot = rotation;
}

void CORESwerve::setX(double x) {

}

void CORESwerve::setY(double y) {

}

double CORESwerve::getX() {
    return 0;
}

double CORESwerve::getY() {
    return 0;
}

void CORESwerve::polar(double magnitude, double direction, double rotation) {

}

void CORESwerve::setMagnitude(double magnitude) {

}

void CORESwerve::setDirection(double direction) {

}

double CORESwerve::getMagnitude() {
    return 0;
}

double CORESwerve::getDirection() {
    return 0;
}

void CORESwerve::postTeleopTask() {
    if(true) {
        for (auto i : m_modules) {
            i->setDirection(1);
            double a, b;
            a = m_x + m_rot * i->y;
            b = m_y - m_rot * i->x;
            i->setMagnitude(sqrt(pow(a, 2) + pow(b, 2)));
            double setAngle = (180.0 / 3.1415) * atan2(a, b);
            if (abs(setAngle - i->getCurrentAngle()) > 180) {
                setAngle = i->clamp(setAngle - 180);
                i->setDirection(-1);
            }
            i->setDirection(setAngle);
            i->update();
        }
    }
    if(false) {
        for(auto i : m_modules){
            double a,b;
            a = m_x + m_rot * i->y;
            b = m_y - m_rot * i->x;
            i->setMagnitude(sqrt(pow(a,2) + pow(b,2)));
            i->setDirection((180.0/3.1415)*atan2(a,b));
            i->update();
        }
    }
}
