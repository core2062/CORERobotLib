#include "COREMotor.h"
#include <math.h>
#include "../COREHardware.h"

using namespace std;

using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod, double pProfile1Value,
                     double iProfile1Value, double dProfile1Value, double pProfile2Value, double iProfile2Value,
                     double dProfile2Value, int integralAccuracy) :
        COREPID(m_motorControlMode == VEL_PID ? PIDType::VEL : (m_motorControlMode == POS_PID ? PIDType::POS
                                                                                              : PIDType::POS_VEL),
                pProfile1Value, iProfile1Value, dProfile1Value, pProfile2Value, iProfile2Value, dProfile2Value,
                integralAccuracy),
        m_motorControlMode(controlMethod),
        m_motorControllerType(controller),
        m_motorPort(port),
        m_instance(this)
{
#ifdef __arm__
    if(m_motorControllerType == CORE::CANTALON) {
        std::shared_ptr<CANTalon> pointer(new CANTalon(port));
        CANTalonController = pointer;
    }
    else if(m_motorControllerType == CORE::JAGUAR) {
        std::shared_ptr<Jaguar> pointer(new Jaguar(port));
        JaguarController = pointer;
    }
    else if(m_motorControllerType == CORE::VICTOR) {
        std::shared_ptr<Victor> pointer(new Victor(port));
        VictorController = pointer;
    }
    else {
        //TODO: Throw error
    }
#else
    m_trapSumTimer = new CORETimer();
    m_trapSumTimer->Reset();
    m_trapSumTimer->Start();
#endif
    Robot::addMotor(m_instance);
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
    if (m_motorControlMode == POS_PID) {
		setType(POS);
    } else if (m_motorControlMode == VEL_PID) {
		setType(VEL);
	}
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

void COREMotor::addSlave(std::shared_ptr<COREMotor> slaveMotor) {
    m_slaveMotors.push_back(slaveMotor.get());
}

void COREMotor::motorSafety(bool disableMotorSafety) {
    m_motorSafetyDisabled = disableMotorSafety;
}

void COREMotor::postTeleopTask() {
	//setActualPos(getActualPos());
	//setActualVel(getActualVel());
    if (m_motorControlMode == POS_PID) {
        m_motorValue = calculate();
        m_motorUpdated = true;
    } else if (m_motorControlMode == VEL_PID) {
        m_motorValue = calculate();
        m_motorUpdated = true;
	}
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
    m_motorValue = fabs(m_motorValue) > 1 ? (m_motorValue > 1 ? 1 : -1) : m_motorValue;
    m_motorValue = (m_motorValue < m_deadBandMax && m_motorValue > m_deadBandMin) ? 0 : m_motorValue;
    for (auto motor : m_slaveMotors) {
        motor->Set(m_motorValue);
	}
#ifdef __arm__
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
#else
    cout << "Motor Port " << getPort() << " Value = " << m_motorValue << endl;
    m_trapSum -= 0.5 * m_trapSumTimer->Get() * (m_lastMotorValue + m_motorValue);
    m_trapSumTimer->Reset();
    m_lastMotorValue = m_motorValue;
    cout << "Trap Port " << getPort() << " Value = " << m_trapSum << endl;
#endif
    m_motorUpdated = false;
}
