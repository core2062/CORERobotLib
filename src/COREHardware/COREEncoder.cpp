#include "COREEncoder.h"
#include "COREMotor.h"

using namespace CORE;

COREEncoder::COREEncoder(int portA, int portB, encoderType encoderType, bool reversed) : m_encoderType(encoderType),
                                                                         m_reversed(true) {
    shared_ptr<Encoder> pointer(new Encoder(portA, portB));
    encoder = pointer;
}

COREEncoder::COREEncoder(shared_ptr<COREMotor> motor, encoderType encoder) : m_motor(motor), m_encoderType(encoder) {
    if(m_motor->getControllerType() == CANTALON) {
        cout << "Error: motor in port " << m_motor->getPort() << " is not a CANTalon!" << endl;
    }
}

void COREEncoder::setReversed(bool reversed) {
    m_reversed = reversed;
}

bool COREEncoder::getReversed() {
    return m_reversed;
}

int COREEncoder::Get() {
    if(m_motor != nullptr && m_motor->getControllerType() == CANTALON) {
        return (m_motor->CANTalonController->GetEncPosition() * (m_reversed ? -1 : 1)) + m_offset;
    }
    return (encoder->Get() * (m_reversed ? -1 : 1)) + m_offset;
}

double COREEncoder::getDistance(distanceUnit unit) {
    return 0;
}

void COREEncoder::setDistance(distanceUnit unit) {

}

void COREEncoder::Set(int value) {
    if(m_motor != nullptr && m_motor->getControllerType() == CANTALON) {
        m_motor->CANTalonController->SetEncPosition(value);
        m_offset = 0;
    }
    else {
        encoder->Reset();
        m_offset = value;
    }
}

void COREEncoder::Reset() {

}
