#include "COREMotor.h"
#include <COREHardware.h>

using namespace std;

using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod) :
        m_motorControlMode(controlMethod), m_motorControllerType(controller), m_motorPort(port) {
    if(m_motorControllerType == CORE::CANTALON) {
        CANTalonController = make_shared<CANTalon>(port);
    }
    else if(m_motorControllerType == CORE::JAGUAR) {
        JaguarController = make_shared<Jaguar>(port);
    }
    else if(m_motorControllerType == CORE::VICTOR) {
        VictorController = make_shared<Victor>(port);
    }
    else {
        //TODO: Throw error
    }
//    Robot::addMotor(this);
}

void COREMotor::Set(double motorSetValue) {
    m_motorValue = (m_reversed ? -motorSetValue : motorSetValue);
    m_motorUpdated = true;
}

double COREMotor::Get() {
    return m_motorValue;
}

void COREMotor::setReversed(bool reverse) {
    m_reversed = reverse;
}

bool COREMotor::getReversed() {
    return m_reversed;
}

void COREMotor::setControlMode(controlMode controlMethod) {
    m_motorControlMode = controlMethod;
}

controlMode COREMotor::getControlMode() {
    return m_motorControlMode;
}

int COREMotor::getPort() {
    return m_motorPort;
}

controllerType COREMotor::getControllerType() {
    return m_motorControllerType;
}

void COREMotor::setDeadband(double range) {
    setDeadband(-range, range);
}

void COREMotor::setDeadband(double min, double max) {
    m_deadBandMin = min < -1 ? -1 : min;
    m_deadBandMax = max > 1 ? 1 : max;
}

void COREMotor::motorSafety(bool disableMotorSafety) {
    m_motorSafetyDisabled = disableMotorSafety;
}

double COREMotor::getCurrent() {
    if(m_motorControllerType == CORE::CANTALON) {
        return CANTalonController->GetOutputCurrent();
    } else {
        //TODO: Throw error
        return 0;
    }
}

void COREMotor::update() {
    //setActualPos(getActualPos());
    //setActualVel(getActualVel());
/*    if (m_motorControlMode == POS_PID) {
    	//setActualPos(CANTalonController->GetEncPosition()); //TODO: Fix this
        m_motorValue = calculate();
        m_motorUpdated = true;
    } else if (m_motorControlMode == VEL_PID) {
        m_motorValue = calculate();
        m_motorUpdated = true;
    } else if (m_motorControlMode == CONT_PID) {
    	//setActualPos(CANTalonController->GetEncPosition());
        m_motorValue = calculate();
        m_motorUpdated = true;
        cout << "Actual Pos: " << this->m_actualPos << endl;
        cout << "Set Pos: " << this->m_setPosition << endl;
        cout << "Motor Value: " << m_motorValue << endl;
    }*/
    if (!m_motorUpdated && !m_motorSafetyDisabled) {
        if (m_motorSafetyCounter > 3) {
            m_motorValue = 0;
            cout << "Motor not updated!" << endl;
        }
        else {
            m_motorSafetyCounter++;
        }
    }
    else {
        m_motorSafetyCounter = 0;
    }
    m_motorValue = abs(m_motorValue) > 1 ? (m_motorValue > 1 ? 1 : -1) : m_motorValue;
    m_motorValue = (m_motorValue < m_deadBandMax && m_motorValue > m_deadBandMin) ? 0 : m_motorValue;

    if(m_motorControllerType == CORE::CANTALON) {
        CANTalonController->Set(m_motorValue);
    }
    else if(m_motorControllerType == CORE::JAGUAR) {
        JaguarController->Set(m_motorValue);
    }
    else if(m_motorControllerType == CORE::VICTOR) {
        VictorController->Set(m_motorValue);
    }
    else {
        //TODO: Throw error
    }
/*#else
    cout << "Motor Port " << getPort() << " Value = " << m_motorValue << endl;
    m_trapSum -= 0.5 * m_trapSumTimer->Get() * (m_lastMotorValue + m_motorValue);
    m_trapSumTimer->Reset();
    m_lastMotorValue = m_motorValue;
    cout << "Trap Port " << getPort() << " Value = " << m_trapSum << endl;
#endif*/
    m_motorUpdated = false;
}

double COREMotor::PIDGetPos() {
    if(m_motorControllerType == CORE::CANTALON) {
        return CANTalonController->GetEncPosition();
    } else {
        cout << "Error! Motor controller is of type: " << (m_motorControllerType == CORE::JAGUAR ? "Jaguar" : "Victor")
             << " and has no attached encoder!" << endl;
        return 0;
    }
}

double COREMotor::PIDGetVel() {
    if(m_motorControllerType == CORE::CANTALON) {
        return CANTalonController->GetEncVel();
    } else {
        cout << "Error! Motor controller is of type: " << (m_motorControllerType == CORE::JAGUAR ? "Jaguar" : "Victor")
             << " and has no attached encoder!" << endl;
        return 0;
    }
}

void COREMotor::PIDSet(double value) {
    Set(value);
}

//COREMotor::COREMotor(int port, bool reversed) : COREEncoder(m_instance, SRX_RELATIVE) {
//
//}
