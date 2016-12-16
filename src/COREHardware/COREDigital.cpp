#include "COREDigital.h"
#include "COREMotor.h"

using namespace CORE;

COREDigital::COREDigital(shared_ptr<COREMotor> motor, limitSwitch position) : m_motor(motor), m_limitSwitchPosition(position) {
    if(m_motor->getControllerType() == CANTALON) {
        cout << "Error: motor in port " << m_motor->getPort() << " is not a CANTalon!" << endl;
    }
}

void COREDigital::Set(bool value) {

}

bool COREDigital::Get() {
    if(m_motor != nullptr) {
        if(m_limitSwitchPosition == FORWARD) {
            return m_motor->CANTalonController->Get();
        }
        else {
            return m_motor->CANTalonController->GetEncPosition();
        }
    }
}
//int IsFwdLimitSwitchClosed()\\\