#include "COREMotor.h"
#include "COREFramework/COREHardwareManager.h"

using namespace std;
using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod) :
        m_motorControlMode(controlMethod), m_motorControllerType(controller), m_motorPort(port) {
    if(m_motorControllerType == CORE::CANTALON) {
    	m_CANTalonController = make_shared<CANTalon>(port);
        m_encoder = make_shared<COREEncoder>(m_CANTalonController, SRX_RELATIVE);
        m_CANTalonController->DisableSoftPositionLimits();
        setControlMode(controlMethod);
    } else if(m_motorControllerType == CORE::JAGUAR) {
        m_JaguarController = make_shared<Jaguar>(port);
    } else if(m_motorControllerType == CORE::VICTOR) {
        m_VictorController = make_shared<Victor>(port);
    } else {
        CORELog::logError("Non existent motor controller type given");
    }
    COREHardwareManager::addMotor(this);
}

void COREMotor::Set(double motorSetValue) {
    m_motorValue = (m_reversed ? -motorSetValue : motorSetValue);
    m_motorUpdated = true;
}

double COREMotor::Get() {
    return m_motorValue;
}

double COREMotor::GetLast() {
	return m_lastMotorValue;
}

void COREMotor::setReversed(bool reverse) {
    m_reversed = reverse;
}

bool COREMotor::getReversed() {
    return m_reversed;
}

void COREMotor::setControlMode(controlMode controlMethod) {
    if(m_motorControllerType == CANTALON) {
        switch(controlMethod) {
            case VOLTAGE:
                m_CANTalonController->SetControlMode(CANTalon::ControlMode::kVoltage);
                break;
            case PERCENTAGE:
                m_CANTalonController->SetControlMode(CANTalon::ControlMode::kPercentVbus);
                break;
            case CURRENT:
                m_CANTalonController->SetControlMode(CANTalon::ControlMode::kCurrent);
                break;
        }
    }
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

void COREMotor::setFollower(int port) {
    m_isFollower = true;
    if(m_motorControllerType == CORE::CANTALON) {
        m_CANTalonController->SetControlMode(CANTalon::ControlMode::kFollower);
        m_CANTalonController->Set(port);
    }
}

void COREMotor::motorSafety(bool disableMotorSafety) {
    m_motorSafetyDisabled = disableMotorSafety;
}

double COREMotor::getCurrent() {
    if(m_motorControllerType == CORE::CANTALON) {
        return m_CANTalonController->GetOutputCurrent();
    } else {
        //TODO: Throw error
        return 0;
    }
}

void COREMotor::Update() {
    if(m_isFollower || (m_motorControllerType == CANTALON
                                          && (m_CANTalonController->GetTalonControlMode() == CANTalon::TalonControlMode::kFollowerMode
                                        		  || m_CANTalonController->GetControlMode() == CANTalon::ControlMode::kFollower))) {
        m_isFollower = true;
        return;
    }
    if(!m_motorUpdated && !m_motorSafetyDisabled) {
        if(m_motorSafetyCounter > 3) {
            m_motorValue = 0;
            CORELog::logWarning("Motor in port: " + to_string(m_motorPort) + " not updated!");
        } else {
            m_motorSafetyCounter++;
        }
    } else {
        m_motorSafetyCounter = 0;
    }
    m_motorValue = abs(m_motorValue) > 1 ? (m_motorValue > 1 ? 1 : -1) : m_motorValue;
    m_motorValue = (m_motorValue < m_deadBandMax && m_motorValue > m_deadBandMin) ? 0 : m_motorValue;

    if(m_motorControllerType == CORE::CANTALON) {
        m_CANTalonController->Set(m_motorValue);
    } else if(m_motorControllerType == CORE::JAGUAR) {
        m_JaguarController->Set(m_motorValue);
    } else if(m_motorControllerType == CORE::VICTOR) {
        m_VictorController->Set(m_motorValue);
    } else {
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

void COREMotor::ControllerSet(double value) {
    Set(value);
}

shared_ptr<CANTalon> COREMotor::getCANTalon() {
    if(!m_CANTalonController) {
        CORELog::logError("Motor in port: " + to_string(m_motorPort) + " returning CANTalon nullptr!");
    }
    return m_CANTalonController;
}

shared_ptr<Jaguar> COREMotor::getJaguar() {
    if(!m_JaguarController) {
        CORELog::logError("Motor in port: " + to_string(m_motorPort) + " returning Jaguar nullptr!");
    }
    return m_JaguarController;
}

shared_ptr<Victor> COREMotor::getVictor() {
    if(!m_VictorController) {
        CORELog::logError("Motor in port: " + to_string(m_motorPort) + " returning Victor nullptr!");
    }
    return m_VictorController;
}

shared_ptr<COREEncoder> COREMotor::getEncoder() {
    if(!m_encoder) {
        CORELog::logError("Motor in port: " + to_string(m_motorPort) + " returning Encoder nullptr!");
    }
    return m_encoder;
}
